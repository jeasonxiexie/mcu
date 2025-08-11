/**
 * @file lcd_test_fixed.c
 * @brief 修正版LCD测试 - 应用正确的引脚映射和电源控制
 * @version 2.0 - 采用 pin_config.h 和完整的 ST7735S 初始化序列
 */

#include "main.h"
#include "pin_config.h"

/* 基本延时函数 */
static void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

/* GPIO控制宏 - 这些现在会使用 pin_config.h 中的正确定义 */
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

/* 背光控制 (根据 pin_config.h) - PB1(高亮), 低电平有效 */
#define LCD_BL_ON()     HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_RESET) 
#define LCD_BL_OFF()    HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_SET)


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

/* 完整的ST7735S初始化序列 */
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
    LCD_WriteData(0xC8);  // MY, MX, MV, ML, BGR, MH -> 1100 1000, BGR color filter
    
    /* Interface Pixel Format */
    LCD_WriteCommand(0x3A);
    LCD_WriteData(0x05);  // 16-bit/pixel
    
    /* Gamma Correction */
    LCD_WriteCommand(0xE0);
    LCD_WriteData(0x02); LCD_WriteData(0x1C); LCD_WriteData(0x07); LCD_WriteData(0x12);
    LCD_WriteData(0x37); LCD_WriteData(0x32); LCD_WriteData(0x29); LCD_WriteData(0x2D);
    LCD_WriteData(0x29); LCD_WriteData(0x25); LCD_WriteData(0x2B); LCD_WriteData(0x39);
    LCD_WriteData(0x00); LCD_WriteData(0x01); LCD_WriteData(0x03); LCD_WriteData(0x10);
    
    LCD_WriteCommand(0xE1);
    LCD_WriteData(0x03); LCD_WriteData(0x1D); LCD_WriteData(0x07); LCD_WriteData(0x06);
    LCD_WriteData(0x2E); LCD_WriteData(0x2C); LCD_WriteData(0x29); LCD_WriteData(0x2D);
    LCD_WriteData(0x2E); LCD_WriteData(0x2E); LCD_WriteData(0x37); LCD_WriteData(0x3F);
    LCD_WriteData(0x00); LCD_WriteData(0x00); LCD_WriteData(0x02); LCD_WriteData(0x10);
    
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
    /* Column Address Set, add offset 26 for some screens */
    LCD_WriteCommand(0x2A);
    LCD_WriteData(0x00);
    LCD_WriteData(x0 + 26);
    LCD_WriteData(0x00);
    LCD_WriteData(x1 + 26);
    
    /* Row Address Set, add offset 1 for some screens */
    LCD_WriteCommand(0x2B);
    LCD_WriteData(0x00);
    LCD_WriteData(y0 + 1);
    LCD_WriteData(0x00);
    LCD_WriteData(y1 + 1);
    
    /* Memory Write */
    LCD_WriteCommand(0x2C);
}

/* 填充屏幕颜色 */
static void LCD_FillScreen(uint16_t color)
{
    LCD_SetWindow(0, 0, 79, 159);
    
    LCD_CS_LOW();
    LCD_RS_HIGH(); // Data mode
    for(uint32_t i = 0; i < 80 * 160; i++) {
        SPI_WriteByte(color >> 8);
        SPI_WriteByte(color & 0xFF);
    }
    LCD_CS_HIGH();
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

int main(void)
{
    /* HAL初始化 */
    HAL_Init();
    
    /* 配置系统时钟 */
    SystemClock_Config();
    
    /* 使能所有用到的GPIO时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    /* 1. 配置 LCD 电源引脚 (PA3) 并开启 */
    GPIO_InitStruct.Pin = CON_POW_LCD_PIN;
    HAL_GPIO_Init(CON_POW_LCD_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, GPIO_PIN_RESET); // 低电平开启LCD电源
    delay_ms(50); // 等待电源稳定

    /* 2. 配置背光引脚 (PB1) */
    GPIO_InitStruct.Pin = CLD_BL1_PIN;
    HAL_GPIO_Init(CLD_BL1_PORT, &GPIO_InitStruct);
    
    /* 3. 配置LCD控制引脚 (全部在GPIOC) */
    GPIO_InitStruct.Pin = LCD_RST_PIN | LCD_RS_PIN | LCD_SCL_PIN | LCD_CS_PIN | LCD_SDA_PIN;
    HAL_GPIO_Init(LCD_RST_PORT, &GPIO_InitStruct); // All are on GPIOC

    /* 4. 设置引脚初始状态 */
    LCD_CS_HIGH();
    LCD_RST_HIGH();
    LCD_SCL_HIGH();
    LCD_RS_HIGH();
    
    /* 5. 开启背光 */
    LCD_BL_ON();
    delay_ms(100);
    
    /* 6. 初始化LCD */
    LCD_Init();
    
    /* 测试模式 - 循环显示颜色 */
    while(1) {
        LCD_FillScreen(0xF800);  // 红色
        delay_ms(1000);
        LCD_FillScreen(0x07E0);  // 绿色
        delay_ms(1000);
        LCD_FillScreen(0x001F);  // 蓝色
        delay_ms(1000);
    }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
