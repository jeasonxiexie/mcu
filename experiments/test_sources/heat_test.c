/**
 * @file heat_test.c
 * @brief Basic heat generation test - verify MCU is running
 */

#include "wb32l003_hal.h"

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Initialize HAL */
    HAL_Init();
    
    /* Configure power latch - PC6 LOW */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
    
    /* Configure LED - PB15 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* Configure all LCD pins as outputs toggling */
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* Main loop - generate maximum heat */
    volatile uint32_t counter = 0;
    volatile uint32_t dummy = 0;
    
    while (1)
    {
        /* Toggle LED rapidly */
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
        
        /* Toggle all LCD pins rapidly to generate heat */
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
        
        /* CPU intensive operations */
        for(uint32_t i = 0; i < 10000; i++)
        {
            dummy = i * i * i;
            counter += dummy;
            
            /* More GPIO toggles */
            if(i % 100 == 0)
            {
                GPIOB->ODR ^= 0xFF00;  /* Toggle upper 8 pins */
            }
        }
        
        /* Avoid optimization */
        if(counter > 0xFFFFFF00)
        {
            counter = 0;
        }
    }
}