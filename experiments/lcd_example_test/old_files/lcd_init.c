#include "lcd_init.h"
#include "wb32l003_hal.h"

// 延时函数
static void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

// GPIO初始化
void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 使能GPIO时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    // 首先配置LCD电源控制引脚PA3，并立即使能LCD电源
    GPIO_InitStruct.Pin = LCD_PWR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_PWR_PORT, &GPIO_InitStruct);
    LCD_PWR_On();  // 立即使能LCD电源
    
    // 配置GPIOC引脚 (LCD接口)
    GPIO_InitStruct.Pin = LCD_SCLK_PIN | LCD_MOSI_PIN | LCD_CS_PIN | 
                          LCD_DC_PIN | LCD_RES_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    // 配置GPIOB引脚 (背光控制)
    GPIO_InitStruct.Pin = LCD_BL1_PIN | LCD_BL2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // 设置初始状态
    LCD_CS_Set();
    LCD_DC_Set();
    LCD_SCLK_Set();
    LCD_MOSI_Set();
    LCD_RES_Set();
    LCD_BL1_Set();  // PNP三极管，高电平关闭
    LCD_BL2_Clr();  // 低亮度默认关闭
}

// 软件SPI写入一个字节
void LCD_Writ_Bus(uint8_t dat)
{
    uint8_t i;
    LCD_CS_Clr();
    
    for(i = 0; i < 8; i++)
    {
        LCD_SCLK_Clr();
        if(dat & 0x80)
        {
            LCD_MOSI_Set();
        }
        else
        {
            LCD_MOSI_Clr();
        }
        LCD_SCLK_Set();
        dat <<= 1;
    }
    
    LCD_CS_Set();
}

// 写入8位数据
void LCD_WR_DATA8(uint8_t dat)
{
    LCD_Writ_Bus(dat);
}

// 写入16位数据
void LCD_WR_DATA(uint16_t dat)
{
    LCD_Writ_Bus(dat >> 8);
    LCD_Writ_Bus(dat);
}

// 写入寄存器/命令
void LCD_WR_REG(uint8_t dat)
{
    LCD_DC_Clr();  // 命令模式
    LCD_Writ_Bus(dat);
    LCD_DC_Set();  // 数据模式
}

// 设置地址范围
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if(USE_HORIZONTAL == 0)
    {
        LCD_WR_REG(0x2a);  // 列地址设置
        LCD_WR_DATA(x1 + 24);
        LCD_WR_DATA(x2 + 24);
        LCD_WR_REG(0x2b);  // 行地址设置
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);  // 内存写入
    }
    else if(USE_HORIZONTAL == 1)
    {
        LCD_WR_REG(0x2a);
        LCD_WR_DATA(x1 + 24);
        LCD_WR_DATA(x2 + 24);
        LCD_WR_REG(0x2b);
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);
    }
    else if(USE_HORIZONTAL == 2)
    {
        LCD_WR_REG(0x2a);
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);
        LCD_WR_DATA(y1 + 24);
        LCD_WR_DATA(y2 + 24);
        LCD_WR_REG(0x2c);
    }
    else
    {
        LCD_WR_REG(0x2a);
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);
        LCD_WR_DATA(y1 + 24);
        LCD_WR_DATA(y2 + 24);
        LCD_WR_REG(0x2c);
    }
}

// LCD初始化
void LCD_Init(void)
{
    LCD_GPIO_Init();  // 初始化GPIO
    
    // 硬件复位
    LCD_RES_Clr();
    delay_ms(100);
    LCD_RES_Set();
    delay_ms(100);
    
    // 打开背光（高亮度 - PNP三极管低电平导通）
    LCD_BL1_Clr();
    delay_ms(100);
    
    // ST7735S初始化序列
    LCD_WR_REG(0x11);  // Sleep out
    delay_ms(120);
    
    // Frame rate control
    LCD_WR_REG(0xB1);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    
    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    
    LCD_WR_REG(0xB3);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    
    LCD_WR_REG(0xB4);  // Dot inversion
    LCD_WR_DATA8(0x03);
    
    // Power control
    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x04);
    
    LCD_WR_REG(0xC1);
    LCD_WR_DATA8(0xC5);
    
    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x0d);
    LCD_WR_DATA8(0x00);
    
    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0x2A);
    
    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0xEE);
    
    LCD_WR_REG(0xC5);  // VCOM
    LCD_WR_DATA8(0x06);
    
    // Memory access control
    LCD_WR_REG(0x36);
    if(USE_HORIZONTAL == 0) LCD_WR_DATA8(0x08);
    else if(USE_HORIZONTAL == 1) LCD_WR_DATA8(0xC8);
    else if(USE_HORIZONTAL == 2) LCD_WR_DATA8(0x78);
    else LCD_WR_DATA8(0xA8);
    
    // Color format
    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x55);  // 16-bit color
    
    // Gamma correction
    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0x0b);
    LCD_WR_DATA8(0x17);
    LCD_WR_DATA8(0x0a);
    LCD_WR_DATA8(0x0d);
    LCD_WR_DATA8(0x1a);
    LCD_WR_DATA8(0x19);
    LCD_WR_DATA8(0x16);
    LCD_WR_DATA8(0x1d);
    LCD_WR_DATA8(0x21);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x37);
    LCD_WR_DATA8(0x3c);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x10);
    
    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0x0c);
    LCD_WR_DATA8(0x19);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x0d);
    LCD_WR_DATA8(0x1b);
    LCD_WR_DATA8(0x19);
    LCD_WR_DATA8(0x15);
    LCD_WR_DATA8(0x1d);
    LCD_WR_DATA8(0x21);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x39);
    LCD_WR_DATA8(0x3E);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x10);
    
    delay_ms(120);
    LCD_WR_REG(0x29);  // Display on
}

// 填充矩形区域
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1);
    
    for(i = ysta; i < yend; i++)
    {
        for(j = xsta; j < xend; j++)
        {
            LCD_WR_DATA(color);
        }
    }
}