/*
 * o3 建议的电源测试 - 使用内部 8MHz 时钟
 */

#include "main.h"

// 简单的系统时钟配置 - 只用内部 8MHz
static void SystemClock_Config_8MHz(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    // 使用内部 8MHz HSI
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;  // 不使用 PLL
    
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        while(1);  // 时钟配置失败
    }
    
    // 系统时钟设为 HSI
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        while(1);  // 时钟配置失败
    }
}

// 简单的延时函数
static void simple_delay(uint32_t count)
{
    for(volatile uint32_t i = 0; i < count; i++) {
        __NOP();
    }
}

int main(void)
{
    /* PC6 已在 Early_PowerLatch() 中设置为 LOW */
    
    /* 初始化 HAL 库 */
    HAL_Init();
    
    /* 使用简单的 8MHz 内部时钟 */
    SystemClock_Config_8MHz();
    
    /* 初始化所有 GPIO */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    // 手动启用 GPIOD
    RCC->AHBENR |= (1UL << 20);
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* PA3 - LCD 电源控制 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* PB1 - 背光电源控制 */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* PB2 - 背光亮度控制 */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* PC0 - 测试输出（如果有 LED） */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /* 根据 o3 分析设置电源 */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);  // PA3 LOW = LCD ON
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);  // PB1 LOW = 背光供电
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);    // PB2 HIGH = 高亮
    
    /* 主循环 - 切换 PC0 作为心跳指示 */
    uint8_t toggle = 0;
    
    while (1)
    {
        /* 保持 PC6 LOW - 虽然应该已经被锁存 */
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
        
        /* 切换 PC0 */
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, toggle ? GPIO_PIN_SET : GPIO_PIN_RESET);
        toggle = !toggle;
        
        /* 延时约 500ms (8MHz 时钟) */
        simple_delay(1000000);
    }
}