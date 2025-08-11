/**
 * @file lcd_example.c
 * @brief LCD测试程序 - 基于STM32F030例程移植到WB32L003
 * @note 使用软件SPI驱动ST7735S LCD
 */

#include "wb32l003.h"
#include "hal/wb32l003_hal.h"
#include "lcd_init.h"

// 系统时钟配置
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    // 配置内部HSI振荡器 (48MHz)
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    
    // 配置系统时钟
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | 
                                  RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}

// 早期电源初始化 - 与 v1 docs 对齐：锁存放在最早期 Early_PowerLatch
extern void Early_PowerLatch(void);
void Early_Power_Init(void)
{
    // 确保最早期已锁存（Reset_Handler 调用 Early_PowerLatch）
    Early_PowerLatch();
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 使能GPIOA时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    // 配置PA3为输出，用于LCD电源控制
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // 立即使能LCD电源
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
}

// LED指示灯初始化（用于调试）
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 使能GPIOB时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    // 配置PB0为LED输出
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// 简单的显示测试
void LCD_Test(void)
{
    // 全屏填充红色
    LCD_Fill(0, 0, LCD_W, LCD_H, RED);
    HAL_Delay(1000);
    
    // 全屏填充绿色
    LCD_Fill(0, 0, LCD_W, LCD_H, GREEN);
    HAL_Delay(1000);
    
    // 全屏填充蓝色
    LCD_Fill(0, 0, LCD_W, LCD_H, BLUE);
    HAL_Delay(1000);
    
    // 显示彩条
    LCD_Fill(0, 0, LCD_W/3, LCD_H, RED);
    LCD_Fill(LCD_W/3, 0, LCD_W*2/3, LCD_H, GREEN);
    LCD_Fill(LCD_W*2/3, 0, LCD_W, LCD_H, BLUE);
}

int main(void)
{
    // CRITICAL: Lock power FIRST before anything else!
    // This must be done immediately to prevent power loss
    Early_Power_Init();
    
    // HAL库初始化
    HAL_Init();
    
    // 系统时钟配置
    SystemClock_Config();
    
    // LED初始化（调试用）
    LED_Init();
    
    // LCD初始化
    LCD_Init();
    
    // LED闪烁3次表示启动成功
    for(int i = 0; i < 3; i++)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_Delay(100);
    }
    
    // 主循环
    while(1)
    {
        // 执行LCD测试
        LCD_Test();
        
        // LED闪烁表示运行中
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(500);
    }
}

// 错误处理
void Error_Handler(void)
{
    // 错误时快速闪烁LED
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(100);
    }
}

// 断言失败处理 - 注释掉避免重复定义
// void assert_failed(uint8_t *file, uint32_t line)
// {
//     Error_Handler();
// }