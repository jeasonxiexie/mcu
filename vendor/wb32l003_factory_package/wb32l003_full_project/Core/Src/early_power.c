/**
 * @file early_power.c
 * @brief Early power latch implementation
 * 
 * This function must be called as early as possible to maintain power
 */

#include "wb32l003.h"

/**
 * @brief Early power latch - called from Reset_Handler
 * 
 * PC6 must be set HIGH within 150ms of power-on to maintain power.
 * This is called before any other initialization.
 */
void Early_PowerLatch(void) {
    /* Enable GPIOC clock first */
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    /* Configure PC6 as output push-pull */
    GPIOC->MODER &= ~(3UL << (6 * 2));
    GPIOC->MODER |= (1UL << (6 * 2));
    
    /* Set PC6 HIGH to maintain power */
    GPIOC->BSRR = (1UL << 6);
}