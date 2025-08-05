#include "wb32l003.h"
#include "pin_config.h"
#include "st7735.h"

// 简单的延时函数
void simple_delay(uint32_t delay)
{
    for(uint32_t i = 0; i < delay * 1000; i++) {
        __NOP();
    }
}

int main(void)
{
    // 基本初始化
    HAL_Init();
    
    // 使能所有 GPIO 时钟
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN;
    
    // 1. 配置 LED3 (PD0) 为输出，用于生命信号
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0;  // PD0
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // 2. 配置 MODE 按键 (PC4) 为输入
    GPIO_InitStruct.Pin = KEY_MODE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY_MODE_PORT, &GPIO_InitStruct);
    
    // 3. 强制开启 TFT 背光 (PA1) - 直接拉高
    GPIO_InitStruct.Pin = TFT_BL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TFT_BL_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(TFT_BL_PORT, TFT_BL_PIN, GPIO_PIN_SET); // 背光常亮
    
    // 4. 初始化 SPI 和 TFT
    // 配置 SPI 引脚
    GPIO_InitStruct.Pin = TFT_SPI_SCLK_PIN | TFT_SPI_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;  // PC5/PC6 -> SPI1
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    // 配置 TFT 控制引脚
    GPIO_InitStruct.Pin = TFT_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TFT_CS_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TFT_DC_PIN;
    HAL_GPIO_Init(TFT_DC_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TFT_RST_PIN;
    HAL_GPIO_Init(TFT_RST_PORT, &GPIO_InitStruct);
    
    // CS 默认高电平
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
    
    // 初始化 SPI
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI_HandleTypeDef hspi1 = {0};
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    HAL_SPI_Init(&hspi1);
    
    // 5. 尝试初始化 TFT
    HAL_Delay(100); // 等待电源稳定
    ST7735_Init();
    
    // 6. 填充屏幕为红色作为测试
    ST7735_FillScreen(ST7735_RED);
    
    // 7. 显示简单文字
    ST7735_FillRect(10, 10, 60, 20, ST7735_WHITE);
    ST7735_FillRect(10, 40, 60, 20, ST7735_GREEN);
    
    // 主循环
    uint32_t counter = 0;
    uint8_t led_state = 0;
    
    while (1)
    {
        // LED3 每秒闪烁一次
        if (++counter >= 1000)
        {
            counter = 0;
            led_state = !led_state;
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, led_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
            
            // 在屏幕上也显示计数
            static uint8_t screen_color = 0;
            screen_color++;
            if (screen_color == 1) ST7735_FillRect(0, 0, 20, 20, ST7735_RED);
            else if (screen_color == 2) ST7735_FillRect(0, 0, 20, 20, ST7735_GREEN);
            else if (screen_color == 3) ST7735_FillRect(0, 0, 20, 20, ST7735_BLUE);
            else { 
                screen_color = 0;
                ST7735_FillRect(0, 0, 20, 20, ST7735_WHITE);
            }
        }
        
        // 检查 MODE 按键
        if (HAL_GPIO_ReadPin(KEY_MODE_PORT, KEY_MODE_PIN) == GPIO_PIN_RESET)
        {
            // 按键按下时，屏幕变黄色
            ST7735_FillScreen(ST7735_YELLOW);
            HAL_Delay(200); // 简单消抖
        }
        
        HAL_Delay(1);
    }
}