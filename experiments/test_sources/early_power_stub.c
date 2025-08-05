/**
 * @file early_power_stub.c
 * @brief Stub implementation for Early_PowerLatch
 */

#include "wb32l003_hal.h"

void Early_PowerLatch(void)
{
    /* Enable GPIOC clock */
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    /* Configure PC6 as output */
    GPIOC->MODER &= ~(3U << (6 * 2));
    GPIOC->MODER |= (1U << (6 * 2));
    
    /* Set PC6 LOW to latch power */
    GPIOC->BRR = GPIO_PIN_6;
}