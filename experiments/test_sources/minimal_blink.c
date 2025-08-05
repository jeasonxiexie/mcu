/**
 * @file minimal_blink.c
 * @brief Absolute minimal test - just blink LED
 */

#include "wb32l003.h"

/* Simple delay */
void delay(uint32_t count)
{
    volatile uint32_t i;
    for(i = 0; i < count; i++);
}

int main(void)
{
    /* Enable GPIOB and GPIOC clocks directly via registers */
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
    
    /* Configure PC6 as output (power latch) */
    GPIOC->MODER &= ~(3U << (6 * 2));  /* Clear mode bits */
    GPIOC->MODER |= (1U << (6 * 2));   /* Set as output */
    GPIOC->BRR = (1U << 6);            /* Set PC6 LOW */
    
    /* Configure PB15 as output (LED) */
    GPIOB->MODER &= ~(3U << (15 * 2)); /* Clear mode bits */
    GPIOB->MODER |= (1U << (15 * 2));  /* Set as output */
    
    /* Simple blink loop */
    while(1)
    {
        /* LED ON */
        GPIOB->BSRR = (1U << 15);
        delay(500000);
        
        /* LED OFF */
        GPIOB->BRR = (1U << 15);
        delay(500000);
    }
}

/* Minimal Early_PowerLatch */
void Early_PowerLatch(void)
{
    /* Enable GPIOC clock */
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    /* Configure PC6 as output */
    GPIOC->MODER &= ~(3U << (6 * 2));
    GPIOC->MODER |= (1U << (6 * 2));
    
    /* Set PC6 LOW to latch power */
    GPIOC->BRR = (1U << 6);
}