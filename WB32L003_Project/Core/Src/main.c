/**
 * @file main_lcd_test_v9.c
 * @brief LCD测试 v9.0 - 使用正确的GPIOC引脚
 * @version 9.0 - 基于原理图的正确引脚映射
 */

#include "main.h"
#include "pin_config.h"  // 使用统一的引脚定义

/* 延时函数使用HAL_Delay */
#define delay_ms(x) HAL_Delay(x)

/* GPIO控制宏 - 使用pin_config.h中的定义 */
#define LCD_CS_LOW()    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()   HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_RS_LOW()    HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET)
#define LCD_RS_HIGH()   HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET)
#define LCD_RST_LOW()   HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)
#define LCD_RST_HIGH()  HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)
#define LCD_SCL_LOW()   HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_RESET)
#define LCD_SCL_HIGH()  HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_SET)
#define LCD_SDA_LOW()   HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_RESET)
#define LCD_SDA_HIGH()  HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_SET)

/* 背光控制 */
#define LCD_BL1_ON()    HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_RESET)  // PNP晶体管，低电平导通
#define LCD_BL1_OFF()   HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_SET)
#define LCD_BL2_ON()    HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_RESET)
#define LCD_BL2_OFF()   HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_SET)

/* 软件SPI写字节 */
static void SPI_WriteByte(uint8_t data)
{
    for(int i = 7; i >= 0; i--) {
        LCD_SCL_LOW();
        if(data & (1 << i)) {
            LCD_SDA_HIGH();
        } else {
            LCD_SDA_LOW();
        }
        LCD_SCL_HIGH();
    }
}

/* 写命令到LCD */
static void LCD_WriteCommand(uint8_t cmd)
{
    LCD_CS_LOW();
    LCD_RS_LOW();  // 命令模式
    SPI_WriteByte(cmd);
    LCD_CS_HIGH();
}

/* 写数据到LCD */
static void LCD_WriteData(uint8_t data)
{
    LCD_CS_LOW();
    LCD_RS_HIGH();  // 数据模式
    SPI_WriteByte(data);
    LCD_CS_HIGH();
}

/* ST7735S初始化序列 */
static void ST7735_Init(void)
{
    /* 硬件复位 */
    LCD_RST_HIGH();
    delay_ms(10);
    LCD_RST_LOW();
    delay_ms(10);
    LCD_RST_HIGH();
    delay_ms(120);
    
    /* 软件复位 */
    LCD_WriteCommand(0x01);  // SWRESET
    delay_ms(150);
    
    /* 退出睡眠模式 */
    LCD_WriteCommand(0x11);  // SLPOUT
    delay_ms(120);
    
    /* Frame Rate Control */
    LCD_WriteCommand(0xB1);
    LCD_WriteData(0x01);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x2D);
    
    LCD_WriteCommand(0xB2);
    LCD_WriteData(0x01);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x2D);
    
    LCD_WriteCommand(0xB3);
    LCD_WriteData(0x01);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x2D);
    LCD_WriteData(0x01);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x2D);
    
    /* Display Inversion Control */
    LCD_WriteCommand(0xB4);
    LCD_WriteData(0x07);
    
    /* Power Control */
    LCD_WriteCommand(0xC0);
    LCD_WriteData(0xA2);
    LCD_WriteData(0x02);
    LCD_WriteData(0x84);
    
    LCD_WriteCommand(0xC1);
    LCD_WriteData(0xC5);
    
    LCD_WriteCommand(0xC2);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x00);
    
    LCD_WriteCommand(0xC3);
    LCD_WriteData(0x8A);
    LCD_WriteData(0x2A);
    
    LCD_WriteCommand(0xC4);
    LCD_WriteData(0x8A);
    LCD_WriteData(0xEE);
    
    /* VCOM Control */
    LCD_WriteCommand(0xC5);
    LCD_WriteData(0x0E);
    
    /* Display Inversion Off */
    LCD_WriteCommand(0x20);
    
    /* Memory Data Access Control */
    LCD_WriteCommand(0x36);
    LCD_WriteData(0xC8);
    
    /* Interface Pixel Format */
    LCD_WriteCommand(0x3A);
    LCD_WriteData(0x05);  // 16-bit color
    
    /* Gamma Correction */
    LCD_WriteCommand(0xE0);
    LCD_WriteData(0x02);
    LCD_WriteData(0x1C);
    LCD_WriteData(0x07);
    LCD_WriteData(0x12);
    LCD_WriteData(0x37);
    LCD_WriteData(0x32);
    LCD_WriteData(0x29);
    LCD_WriteData(0x2D);
    LCD_WriteData(0x29);
    LCD_WriteData(0x25);
    LCD_WriteData(0x2B);
    LCD_WriteData(0x39);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x03);
    LCD_WriteData(0x10);
    
    LCD_WriteCommand(0xE1);
    LCD_WriteData(0x03);
    LCD_WriteData(0x1D);
    LCD_WriteData(0x07);
    LCD_WriteData(0x06);
    LCD_WriteData(0x2E);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x29);
    LCD_WriteData(0x2D);
    LCD_WriteData(0x2E);
    LCD_WriteData(0x2E);
    LCD_WriteData(0x37);
    LCD_WriteData(0x3F);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x02);
    LCD_WriteData(0x10);
    
    /* Normal Display Mode On */
    LCD_WriteCommand(0x13);
    delay_ms(10);
    
    /* Display On */
    LCD_WriteCommand(0x29);
    delay_ms(100);
}

/* 设置显示窗口 - 带偏移修正 */
static void LCD_SetWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    /* 添加X轴偏移 - 根据测试可能需要调整 */
    const uint8_t x_offset = 26;  // 京东方LCD常见偏移
    const uint8_t y_offset = 1;
    
    /* Column Address Set */
    LCD_WriteCommand(0x2A);
    LCD_WriteData(0x00);
    LCD_WriteData(x0 + x_offset);
    LCD_WriteData(0x00);
    LCD_WriteData(x1 + x_offset);
    
    /* Row Address Set */
    LCD_WriteCommand(0x2B);
    LCD_WriteData(0x00);
    LCD_WriteData(y0 + y_offset);
    LCD_WriteData(0x00);
    LCD_WriteData(y1 + y_offset);
    
    /* Memory Write */
    LCD_WriteCommand(0x2C);
}

/* 填充屏幕颜色 */
static void LCD_FillScreen(uint16_t color)
{
    LCD_SetWindow(0, 0, 79, 159);  // 80x160分辨率
    
    /* 开始写入数据 */
    LCD_CS_LOW();
    LCD_RS_HIGH();  // 数据模式
    
    for(uint32_t i = 0; i < 80 * 160; i++) {
        SPI_WriteByte(color >> 8);
        SPI_WriteByte(color & 0xFF);
    }
    
    LCD_CS_HIGH();
}

/* 简单系统时钟配置 */
static void SystemClock_Config(void)
{
    /* 使用默认HSI时钟 */
}

int main(void)
{
    /* HAL初始化 */
    HAL_Init();
    
    /* 配置系统时钟 */
    SystemClock_Config();
    
    /* 使能GPIO时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* 紧急：首先锁定主电源！PB7必须立即拉低 */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = GPIO_PIN_7;  // PB7 - CON_POW_CPU
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);  // 拉低PB7，锁定电源！
    
    /* 配置LCD电源控制 - PA3 */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = CON_POW_LCD_PIN;
    HAL_GPIO_Init(CON_POW_LCD_PORT, &GPIO_InitStruct);
    /* 开启LCD电源 (LOW = ON for PNP transistor) */
    HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, GPIO_PIN_RESET);
    
    /* 配置LCD控制引脚 - 全部在GPIOC！ */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    /* LCD引脚在GPIOC - 根据原理图 */
    GPIO_InitStruct.Pin = LCD_RST_PIN | LCD_RS_PIN | LCD_SCL_PIN | 
                         LCD_CS_PIN | LCD_SDA_PIN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  // 注意是GPIOC！
    
    /* 配置背光控制引脚 - GPIOB */
    GPIO_InitStruct.Pin = CLD_BL1_PIN | CLD_BL2_PIN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* 设置初始状态 */
    LCD_CS_HIGH();
    LCD_RST_HIGH();
    LCD_SCL_LOW();
    
    /* 开启背光 */
    LCD_BL1_ON();   // 低电平导通
    LCD_BL2_ON();   // 低电平导通
    
    /* 等待电源稳定 */
    delay_ms(100);
    
    /* 初始化LCD */
    ST7735_Init();
    
    /* 测试模式 - 循环显示颜色 */
    while(1) {
        LCD_FillScreen(0xF800);  // 红色
        delay_ms(1000);
        
        LCD_FillScreen(0x07E0);  // 绿色
        delay_ms(1000);
        
        LCD_FillScreen(0x001F);  // 蓝色
        delay_ms(1000);
        
        LCD_FillScreen(0xFFFF);  // 白色
        delay_ms(1000);
        
        LCD_FillScreen(0x0000);  // 黑色
        delay_ms(1000);
    }

}