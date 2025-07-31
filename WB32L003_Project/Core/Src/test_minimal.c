/**
 * 最小测试程序 - 用于验证 MCU 是否正常运行
 * 功能：
 * 1. PD0 (LED3) 每秒闪烁
 * 2. 背光强制开启
 * 3. 屏幕显示红色
 */

#include "wb32l003.h"
#include "st7735.h"

// 全局 SPI 句柄
extern SPI_HandleTypeDef hspi1;

// 简单延时
void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

int main(void)
{
    // 系统初始化
    HAL_Init();
    
    // GPIO 时钟使能 - 使用 HAL 函数避免寄存器差异
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 初始化 PD0 作为 LED 输出（生命信号）
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // 初始化 PA1 作为背光控制（强制高电平）
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);  // 背光常亮
    
    // 初始化 PC4 作为 MODE 按键输入
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    // 配置 SPI 引脚 - PC5(SCK), PC6(MOSI)
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    // 配置 TFT 控制引脚
    // PC0 - CS
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
    
    // PA3 - DC
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // PD3 - RST
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // 初始化 SPI1
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;  // 慢一点更稳定
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        // SPI 初始化失败，快速闪烁 LED
        while(1)
        {
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_0);
            delay_ms(100);
        }
    }
    
    // 等待电源稳定
    delay_ms(200);
    
    // 初始化 TFT 屏幕
    ST7735_Init();
    
    // 填充整个屏幕为红色
    ST7735_FillScreen(ST7735_RED);
    delay_ms(500);
    
    // 显示开机信息
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_FillRect(10, 10, 60, 20, ST7735_WHITE);
    ST7735_FillRect(10, 40, 60, 20, ST7735_GREEN);
    
    // 主循环
    uint8_t led_state = 0;
    uint32_t last_tick = HAL_GetTick();
    uint8_t color_index = 0;
    
    while (1)
    {
        uint32_t current_tick = HAL_GetTick();
        
        // 每秒切换 LED 状态
        if (current_tick - last_tick >= 1000)
        {
            last_tick = current_tick;
            led_state = !led_state;
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, led_state);
            
            // 在屏幕上显示不同颜色块表示运行
            color_index = (color_index + 1) % 4;
            switch(color_index)
            {
                case 0: ST7735_FillRect(0, 0, 20, 20, ST7735_RED); break;
                case 1: ST7735_FillRect(0, 0, 20, 20, ST7735_GREEN); break;
                case 2: ST7735_FillRect(0, 0, 20, 20, ST7735_BLUE); break;
                case 3: ST7735_FillRect(0, 0, 20, 20, ST7735_YELLOW); break;
            }
        }
        
        // 检测 MODE 按键
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_RESET)
        {
            // 按键按下，整屏变白
            ST7735_FillScreen(ST7735_WHITE);
            delay_ms(200);  // 消抖
            
            // 等待释放
            while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_RESET)
            {
                delay_ms(10);
            }
            
            // 恢复黑色背景
            ST7735_FillScreen(ST7735_BLACK);
        }
    }
}