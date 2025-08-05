/**
 * @file bare_metal_test.c
 * @brief Bare metal test - no HAL dependencies
 * o3 analysis: PC6=LOW, PA3=LOW, PB1=LOW to keep power on
 */

// RCC base address for WB32L003
#define RCC_BASE        0x40021000
#define RCC_AHBENR      (*(volatile unsigned int *)(RCC_BASE + 0x14))

// GPIO base addresses
#define GPIOA_BASE      0x48000000
#define GPIOB_BASE      0x48000400
#define GPIOC_BASE      0x48000800

// GPIO register offsets
#define GPIO_MODER      0x00
#define GPIO_ODR        0x14

// Direct register access macros
#define GPIOA_MODER     (*(volatile unsigned int *)(GPIOA_BASE + GPIO_MODER))
#define GPIOA_ODR       (*(volatile unsigned int *)(GPIOA_BASE + GPIO_ODR))
#define GPIOB_MODER     (*(volatile unsigned int *)(GPIOB_BASE + GPIO_MODER))
#define GPIOB_ODR       (*(volatile unsigned int *)(GPIOB_BASE + GPIO_ODR))
#define GPIOC_MODER     (*(volatile unsigned int *)(GPIOC_BASE + GPIO_MODER))
#define GPIOC_ODR       (*(volatile unsigned int *)(GPIOC_BASE + GPIO_ODR))

void main(void)
{
    // Enable GPIO clocks
    RCC_AHBENR |= (1 << 17);  // GPIOA clock enable
    RCC_AHBENR |= (1 << 18);  // GPIOB clock enable  
    RCC_AHBENR |= (1 << 19);  // GPIOC clock enable
    
    // Small delay for clock stabilization
    for(volatile int i = 0; i < 100; i++);
    
    // CRITICAL: Set PC6 LOW to keep power ON (P-MOS transistor)
    GPIOC_MODER &= ~(3U << (6 * 2));   // Clear mode bits
    GPIOC_MODER |= (1U << (6 * 2));    // Set as output
    GPIOC_ODR &= ~(1 << 6);            // PC6 = LOW (POWER ON!)
    
    // Set PA3 LOW to enable LCD power (PNP transistor)
    GPIOA_MODER &= ~(3U << (3 * 2));   // Clear mode bits
    GPIOA_MODER |= (1U << (3 * 2));    // Set as output
    GPIOA_ODR &= ~(1 << 3);            // PA3 = LOW (LCD ON!)
    
    // Set PB1 LOW to enable backlight power via Q7 PNP
    GPIOB_MODER &= ~(3U << (1 * 2));   // Clear mode bits
    GPIOB_MODER |= (1U << (1 * 2));    // Set as output
    GPIOB_ODR &= ~(1 << 1);            // PB1 = LOW (Backlight ON!)
    
    // Set PB2 HIGH for bright backlight
    GPIOB_MODER &= ~(3U << (2 * 2));   // Clear mode bits
    GPIOB_MODER |= (1U << (2 * 2));    // Set as output
    GPIOB_ODR |= (1 << 2);             // PB2 = HIGH (Bright!)
    
    // Configure LEDs (PC0=Red, PC1=Green, LOW=ON)
    GPIOC_MODER &= ~(3U << (0 * 2));   // Clear PC0 mode
    GPIOC_MODER |= (1U << (0 * 2));    // PC0 as output
    GPIOC_MODER &= ~(3U << (1 * 2));   // Clear PC1 mode
    GPIOC_MODER |= (1U << (1 * 2));    // PC1 as output
    
    // Turn on green LED, turn off red LED
    GPIOC_ODR &= ~(1 << 1);            // PC1 = LOW (Green ON)
    GPIOC_ODR |= (1 << 0);             // PC0 = HIGH (Red OFF)
    
    // Main loop - blink green LED
    while (1)
    {
        // Simple delay
        for(volatile int i = 0; i < 500000; i++);
        
        // Toggle green LED
        GPIOC_ODR ^= (1 << 1);
    }
}

// Minimal interrupt vectors
void Default_Handler(void) { while(1); }

// Stack pointer value (defined in linker script)
extern unsigned long _estack;

// Vector table
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))&_estack,
    main,
    Default_Handler,  // NMI
    Default_Handler,  // HardFault
    0, 0, 0, 0, 0, 0, 0,  // Reserved
    Default_Handler,  // SVC
    0, 0,
    Default_Handler,  // PendSV
    Default_Handler,  // SysTick
    // Add more if needed...
};