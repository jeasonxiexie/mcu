/**
 * @file minimal_test.c
 * @brief Minimal test to verify power control logic
 * Based on o3's analysis: LOW = ON for all power controls
 */

#include "wb32l003.h"

void minimal_main(void)
{
    // Minimal HAL initialization
    HAL_Init();
    
    // Enable GPIOA, GPIOB, GPIOC clocks
    RCC->AHBENR |= (1 << 17);  // GPIOA
    RCC->AHBENR |= (1 << 18);  // GPIOB  
    RCC->AHBENR |= (1 << 19);  // GPIOC
    
    // Small delay for clock stabilization
    for(volatile int i = 0; i < 1000; i++);
    
    // Configure PC6 as output (main power control)
    GPIOC->MODER &= ~(3U << (6 * 2));
    GPIOC->MODER |= (1U << (6 * 2));    // Output mode
    GPIOC->ODR &= ~(1 << 6);            // PC6 = LOW (KEEP POWER ON!)
    
    // Configure PA3 as output (LCD power)
    GPIOA->MODER &= ~(3U << (3 * 2));
    GPIOA->MODER |= (1U << (3 * 2));    // Output mode
    GPIOA->ODR &= ~(1 << 3);            // PA3 = LOW (LCD POWER ON!)
    
    // Configure PB1 as output (backlight power via Q7)
    GPIOB->MODER &= ~(3U << (1 * 2));
    GPIOB->MODER |= (1U << (1 * 2));    // Output mode
    GPIOB->ODR &= ~(1 << 1);            // PB1 = LOW (BACKLIGHT ON!)
    
    // Configure PB2 as output (backlight brightness)
    GPIOB->MODER &= ~(3U << (2 * 2));
    GPIOB->MODER |= (1U << (2 * 2));    // Output mode
    GPIOB->ODR |= (1 << 2);             // PB2 = HIGH (BRIGHT!)
    
    // Optional: Configure LEDs for visual feedback
    // PC0 = Red LED, PC1 = Green LED (LOW = ON)
    GPIOC->MODER &= ~(3U << (0 * 2));
    GPIOC->MODER |= (1U << (0 * 2));    // PC0 Output
    GPIOC->MODER &= ~(3U << (1 * 2));
    GPIOC->MODER |= (1U << (1 * 2));    // PC1 Output
    
    // Turn on Green LED to show we're running
    GPIOC->ODR &= ~(1 << 1);            // PC1 = LOW (GREEN LED ON)
    GPIOC->ODR |= (1 << 0);             // PC0 = HIGH (RED LED OFF)
    
    // Infinite loop - just keep power on
    while (1)
    {
        // Toggle green LED slowly to show we're alive
        static uint32_t counter = 0;
        counter++;
        if (counter > 1000000)
        {
            counter = 0;
            GPIOC->ODR ^= (1 << 1);  // Toggle green LED
        }
    }
}

// Entry point from startup code
int main(void)
{
    minimal_main();
    return 0;
}