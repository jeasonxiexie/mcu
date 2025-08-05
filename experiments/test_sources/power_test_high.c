/**
 * @file power_test_high.c
 * @brief Test with PC6 HIGH for power
 */

#include "wb32l003_hal.h"

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Initialize HAL */
    HAL_Init();
    
    /* Configure power latch - PC6 HIGH (opposite of what we've been doing) */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);  /* PC6 HIGH! */
    
    /* Configure LED - PB15 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* Main loop - blink LED */
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
        HAL_Delay(500);
    }
}

/* Test with PC6 HIGH */
void Early_PowerLatch(void)
{
    /* Enable GPIOC clock */
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    /* Configure PC6 as output */
    GPIOC->MODER &= ~(3U << (6 * 2));
    GPIOC->MODER |= (1U << (6 * 2));
    
    /* Set PC6 HIGH - opposite test */
    GPIOC->BSRR = GPIO_PIN_6;
}