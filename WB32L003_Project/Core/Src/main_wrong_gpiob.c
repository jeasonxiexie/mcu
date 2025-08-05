/**
 * @file lcd_test_fixed.c
 * @brief 修正版LCD测试 - 解决引脚冲突问题
 * @version 1.0 - 修正引脚定义冲突
 */

#include "main.h"

/* 基于specs_comprehensive.md文档，LCD引脚应该在GPIOB */
/* 修正后的引脚定义 - 避免与ADC冲突 */
#define LCD_CS_PIN      GPIO_PIN_0      // PB0 - CS
#define LCD_CS_PORT     GPIOB
#define LCD_RS_PIN      GPIO_PIN_1      // PB1 - DC/RS  
#define LCD_RS_PORT     GPIOB
#define LCD_RST_PIN     GPIO_PIN_2      // PB2 - RESET
#define LCD_RST_PORT    GPIOB
#define LCD_SCL_PIN     GPIO_PIN_3      // PB3 - SCL (避免与原来的冲突)
#define LCD_SCL_PORT    GPIOB  
#define LCD_SDA_PIN     GPIO_PIN_4      // PB4 - SDA
#define LCD_SDA_PORT    GPIOB

/* 背光控制 */
#define LCD_BL_PIN      GPIO_PIN_5      // PB5 - 背光控制
#define LCD_BL_PORT     GPIOB

/* 基本延时函数 */
static void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < ms; i++) {
        for(volatile uint32_t j = 0; j < 3000; j++) {
            __NOP();
        }
    }
}

/* GPIO控制宏 */
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
#define LCD_BL_ON()     HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_SET)
#define LCD_BL_OFF()    HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_RESET)

/* 软件SPI写字节 */
static void SPI_WriteByte(uint8_t data)
{
    for(int i = 7; i >= 0; i--) {
        LCD_SCL_LOW();
        delay_ms(1);  // 增加延时确保时序
        
        if(data & (1 << i)) {
            LCD_SDA_HIGH();
        } else {
            LCD_SDA_LOW();
        }
        delay_ms(1);
        
        LCD_SCL_HIGH();
        delay_ms(1);
    }
    LCD_SCL_LOW();
}

/* 写命令到LCD */
static void LCD_WriteCommand(uint8_t cmd)
{
    LCD_CS_LOW();
    LCD_RS_LOW();  // 命令模式
    delay_ms(1);
    SPI_WriteByte(cmd);
    LCD_CS_HIGH();
    delay_ms(1);
}

/* 写数据到LCD */
static void LCD_WriteData(uint8_t data)
{
    LCD_CS_LOW();
    LCD_RS_HIGH();  // 数据模式
    delay_ms(1);
    SPI_WriteByte(data);
    LCD_CS_HIGH();
    delay_ms(1);
}

/* ST7735S初始化序列 */
static void LCD_Init(void)
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
    
    /* Frame Rate Control (In normal mode/Full colors) */
    LCD_WriteCommand(0xB1);
    LCD_WriteData(0x01);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x2D);
    
    /* Frame Rate Control (In Idle mode/8-colors) */
    LCD_WriteCommand(0xB2);
    LCD_WriteData(0x01);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x2D);
    
    /* Frame Rate Control (In Partial mode/full colors) */
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
    
    /* Power Control 1 */
    LCD_WriteCommand(0xC0);
    LCD_WriteData(0xA2);
    LCD_WriteData(0x02);
    LCD_WriteData(0x84);
    
    /* Power Control 2 */
    LCD_WriteCommand(0xC1);
    LCD_WriteData(0xC5);
    
    /* Power Control 3 (in Normal mode/Full colors) */
    LCD_WriteCommand(0xC2);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x00);
    
    /* Power Control 4 (in Idle mode/8-colors) */
    LCD_WriteCommand(0xC3);
    LCD_WriteData(0x8A);
    LCD_WriteData(0x2A);
    
    /* Power Control 5 (in Partial mode/full colors) */
    LCD_WriteCommand(0xC4);
    LCD_WriteData(0x8A);
    LCD_WriteData(0xEE);
    
    /* VCOM Control 1 */
    LCD_WriteCommand(0xC5);
    LCD_WriteData(0x0E);
    
    /* Display Inversion Off */
    LCD_WriteCommand(0x20);
    
    /* Memory Data Access Control */
    LCD_WriteCommand(0x36);
    LCD_WriteData(0xC8);  // 旋转设置
    
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

/* 设置显示窗口 */
static void LCD_SetWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    /* Column Address Set */
    LCD_WriteCommand(0x2A);
    LCD_WriteData(0x00);
    LCD_WriteData(x0);
    LCD_WriteData(0x00);
    LCD_WriteData(x1);
    
    /* Row Address Set */
    LCD_WriteCommand(0x2B);
    LCD_WriteData(0x00);
    LCD_WriteData(y0);
    LCD_WriteData(0x00);
    LCD_WriteData(y1);
    
    /* Memory Write */
    LCD_WriteCommand(0x2C);
}

/* 填充屏幕颜色 */
static void LCD_FillScreen(uint16_t color)
{
    LCD_SetWindow(0, 0, 79, 159);  // 80x160分辨率
    
    for(uint32_t i = 0; i < 80 * 160; i++) {
        LCD_WriteData(color >> 8);
        LCD_WriteData(color & 0xFF);
    }
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
    
    /* 配置LCD控制引脚 */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    /* LCD引脚在GPIOB */
    GPIO_InitStruct.Pin = LCD_RST_PIN | LCD_RS_PIN | LCD_SCL_PIN | 
                         LCD_CS_PIN | LCD_SDA_PIN | LCD_BL_PIN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* 设置初始状态 */
    LCD_CS_HIGH();
    LCD_RST_HIGH();
    LCD_SCL_HIGH();
    LCD_RS_HIGH();
    LCD_BL_ON();  // 开启背光
    
    delay_ms(100);
    
    /* 初始化LCD */
    LCD_Init();
    
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
        
        LCD_FillScreen(0xFFE0);  // 黄色
        delay_ms(1000);
        
        LCD_FillScreen(0xF81F);  // 紫色
        delay_ms(1000);
        
        LCD_FillScreen(0x07FF);  // 青色
        delay_ms(1000);
    }
}
