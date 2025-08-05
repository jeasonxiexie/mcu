/**
 * @file lcd_diagnostic_test.c
 * @brief LCD诊断测试程序 - 根据o3pro的建议进行系统排查
 * @version 1.0
 */

#include "main.h"
#include "pin_config.h"

/* 延时函数 */
#define delay_ms(x) HAL_Delay(x)

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

/* LED控制 - 注意PC0/PC1与LCD冲突，使用其他LED */
#define LED_TOGGLE()    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0)  // 使用PB0作为测试LED

/* 软件SPI写字节 - 慢速版本便于观察 */
static void SPI_WriteByte_Slow(uint8_t data)
{
    for(int i = 7; i >= 0; i--) {
        LCD_SCL_LOW();
        delay_ms(1);  // 慢速便于逻辑分析仪捕获
        
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
    SPI_WriteByte_Slow(cmd);
    LCD_CS_HIGH();
}

/* 写数据到LCD */
static void LCD_WriteData(uint8_t data)
{
    LCD_CS_LOW();
    LCD_RS_HIGH();  // 数据模式
    SPI_WriteByte_Slow(data);
    LCD_CS_HIGH();
}

/* 诊断步骤1：测试电源控制 */
static void Test_PowerControl(void)
{
    /* 先关闭LCD电源 */
    HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, GPIO_PIN_SET);  // HIGH = OFF
    delay_ms(500);
    
    /* 开启LCD电源 */
    HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, GPIO_PIN_RESET);  // LOW = ON
    delay_ms(500);
    
    /* 背光控制测试 */
    HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_SET);    // OFF
    HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_SET);    // OFF
    delay_ms(500);
    
    HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_RESET);  // ON
    HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_RESET);  // ON
    delay_ms(500);
}

/* 诊断步骤2：测试复位脉冲 */
static void Test_ResetPulse(void)
{
    /* 产生明显的复位脉冲 */
    LCD_RST_HIGH();
    delay_ms(100);
    
    LCD_RST_LOW();   // 复位开始
    delay_ms(50);    // 保持50ms低电平（比正常10ms长，便于观察）
    
    LCD_RST_HIGH();  // 复位结束
    delay_ms(100);
}

/* 诊断步骤3：测试SPI信号 */
static void Test_SPISignals(void)
{
    /* 发送一些测试模式便于逻辑分析仪识别 */
    
    /* 测试1：发送0xAA（10101010）*/
    LCD_WriteCommand(0xAA);
    delay_ms(10);
    
    /* 测试2：发送0x55（01010101）*/
    LCD_WriteCommand(0x55);
    delay_ms(10);
    
    /* 测试3：发送递增序列 */
    for(uint8_t i = 0; i < 8; i++) {
        LCD_WriteData(i);
        delay_ms(5);
    }
}

/* ST7735S初始化（标准） */
static void LCD_Init_ST7735S(void)
{
    Test_ResetPulse();
    
    /* 软件复位 */
    LCD_WriteCommand(0x01);  // SWRESET
    delay_ms(150);
    
    /* 退出睡眠模式 */
    LCD_WriteCommand(0x11);  // SLPOUT
    delay_ms(120);
    
    /* 显示开启 */
    LCD_WriteCommand(0x29);  // DISPON
    delay_ms(100);
    
    /* 填充红色测试 */
    LCD_WriteCommand(0x2A);  // CASET
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x4F);
    
    LCD_WriteCommand(0x2B);  // RASET
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x9F);
    
    LCD_WriteCommand(0x2C);  // RAMWR
    for(int i = 0; i < 80 * 160; i++) {
        LCD_WriteData(0xF8);  // 红色高字节
        LCD_WriteData(0x00);  // 红色低字节
    }
}

/* ST7789初始化（如果是ST7789） */
static void LCD_Init_ST7789(void)
{
    Test_ResetPulse();
    
    /* ST7789特定的初始化序列 */
    LCD_WriteCommand(0x01);  // SWRESET
    delay_ms(150);
    
    LCD_WriteCommand(0x11);  // SLPOUT
    delay_ms(120);
    
    /* ST7789需要的额外配置 */
    LCD_WriteCommand(0x3A);
    LCD_WriteData(0x05);     // 16bit color
    
    LCD_WriteCommand(0x36);
    LCD_WriteData(0x00);     // 扫描方向
    
    LCD_WriteCommand(0x29);  // DISPON
    delay_ms(100);
}

/* 系统时钟配置 */
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
    
    /* 配置测试LED - PB0 */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* LED闪烁3次表示程序开始运行 */
    for(int i = 0; i < 6; i++) {
        LED_TOGGLE();
        delay_ms(200);
    }
    
    /* 配置LCD电源控制 - PA3 */
    GPIO_InitStruct.Pin = CON_POW_LCD_PIN;
    HAL_GPIO_Init(CON_POW_LCD_PORT, &GPIO_InitStruct);
    
    /* 配置LCD控制引脚 - GPIOC */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = LCD_RST_PIN | LCD_RS_PIN | LCD_SCL_PIN | 
                         LCD_CS_PIN | LCD_SDA_PIN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /* 配置背光控制 - GPIOB */
    GPIO_InitStruct.Pin = CLD_BL1_PIN | CLD_BL2_PIN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* 设置初始状态 */
    LCD_CS_HIGH();
    LCD_RST_HIGH();
    LCD_SCL_LOW();
    
    /* 步骤1：测试电源控制 */
    Test_PowerControl();
    
    /* 步骤2：测试复位脉冲 */
    Test_ResetPulse();
    
    /* 步骤3：测试SPI信号 */
    Test_SPISignals();
    
    /* 步骤4：尝试ST7735S初始化 */
    LCD_Init_ST7735S();
    
    /* 主循环：持续发送测试信号 */
    while(1) {
        /* LED闪烁表示程序运行中 */
        LED_TOGGLE();
        
        /* 持续发送SPI测试模式 */
        Test_SPISignals();
        
        delay_ms(1000);
    }
}