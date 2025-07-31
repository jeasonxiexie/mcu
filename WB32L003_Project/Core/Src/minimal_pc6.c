/*
 * Absolute minimal test - just keep PC6 LOW
 */

#include "main.h"

int main(void) {
    // Direct register access - no HAL
    // Enable GPIOC clock
    RCC->AHBENR |= (1UL << 19);  // GPIOCEN
    
    // Small delay
    for(volatile int i = 0; i < 1000; i++);
    
    // Set PC6 as output
    GPIOC->MODER &= ~(3UL << 12);  // Clear bits 12-13
    GPIOC->MODER |= (1UL << 12);   // Set bit 12 (output mode)
    
    // Set PC6 LOW immediately
    GPIOC->BRR = (1UL << 6);  // Reset PC6
    
    // Also try PA3 and PB1 LOW
    RCC->AHBENR |= (1UL << 17);  // GPIOAEN
    RCC->AHBENR |= (1UL << 18);  // GPIOBEN
    
    for(volatile int i = 0; i < 1000; i++);
    
    // PA3 output LOW
    GPIOA->MODER &= ~(3UL << 6);
    GPIOA->MODER |= (1UL << 6);
    GPIOA->BRR = (1UL << 3);
    
    // PB1 output LOW
    GPIOB->MODER &= ~(3UL << 2);
    GPIOB->MODER |= (1UL << 2);
    GPIOB->BRR = (1UL << 1);
    
    // PB2 output HIGH (bright)
    GPIOB->MODER &= ~(3UL << 4);
    GPIOB->MODER |= (1UL << 4);
    GPIOB->BSRR = (1UL << 2);
    
    // Just stay here forever with power ON
    while(1) {
        // Keep PC6 LOW
        GPIOC->BRR = (1UL << 6);
        
        // Toggle PA7 (Red LED) slowly
        GPIOA->MODER &= ~(3UL << 14);
        GPIOA->MODER |= (1UL << 14);
        
        GPIOA->BSRR = (1UL << 7);  // Set
        for(volatile int i = 0; i < 500000; i++);
        
        GPIOA->BRR = (1UL << 7);   // Reset
        for(volatile int i = 0; i < 500000; i++);
    }
}