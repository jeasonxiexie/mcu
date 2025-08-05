/*
 * Ultra-minimal power control debug test
 * Cycles through ALL possible power pin combinations
 */

// Define basic types
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

// Direct register manipulation - no HAL dependencies
#define GPIOA_BASE  0x48000000UL
#define GPIOB_BASE  0x48000400UL
#define GPIOC_BASE  0x48000800UL
#define GPIOD_BASE  0x48000C00UL
#define RCC_BASE    0x40021000UL

// GPIO registers
#define GPIO_MODER(base)   (*(volatile uint32_t*)((base) + 0x00))
#define GPIO_ODR(base)     (*(volatile uint32_t*)((base) + 0x14))
#define GPIO_BSRR(base)    (*(volatile uint32_t*)((base) + 0x18))

// RCC registers
#define RCC_AHBENR         (*(volatile uint32_t*)(RCC_BASE + 0x14))

// Simple delay
static void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 1000; i++) {
        __asm__("nop");
    }
}

// Set pin as output
static void gpio_output(uint32_t gpio_base, uint8_t pin) {
    uint32_t moder = GPIO_MODER(gpio_base);
    moder &= ~(3UL << (pin * 2));  // Clear mode bits
    moder |= (1UL << (pin * 2));   // Set as output
    GPIO_MODER(gpio_base) = moder;
}

// Set pin state
static void gpio_write(uint32_t gpio_base, uint8_t pin, uint8_t state) {
    if (state) {
        GPIO_BSRR(gpio_base) = (1UL << pin);  // Set
    } else {
        GPIO_BSRR(gpio_base) = (1UL << (pin + 16));  // Reset
    }
}

int main(void) {
    // Enable ALL GPIO clocks
    RCC_AHBENR |= (1UL << 17);  // GPIOAEN
    RCC_AHBENR |= (1UL << 18);  // GPIOBEN
    RCC_AHBENR |= (1UL << 19);  // GPIOCEN
    RCC_AHBENR |= (1UL << 20);  // GPIODEN
    
    // Wait for clocks
    delay_ms(10);
    
    // Configure ALL potential power pins as outputs
    gpio_output(GPIOC_BASE, 6);   // PC6 - Main power (o3: LOW=ON)
    gpio_output(GPIOA_BASE, 3);   // PA3 - LCD power (o3: LOW=ON)
    gpio_output(GPIOB_BASE, 1);   // PB1 - Backlight enable (o3: LOW=ON)
    gpio_output(GPIOB_BASE, 2);   // PB2 - Backlight brightness
    
    // Also configure potential missing power pins
    gpio_output(GPIOD_BASE, 0);   // PD0 - Maybe undocumented power?
    gpio_output(GPIOD_BASE, 1);   // PD1 - Maybe undocumented power?
    gpio_output(GPIOA_BASE, 0);   // PA0 - Power button input, but test as output
    gpio_output(GPIOB_BASE, 0);   // PB0 - Physical button, but test
    
    // LED pins for visual feedback
    gpio_output(GPIOA_BASE, 7);   // PA7 - Red LED
    gpio_output(GPIOD_BASE, 3);   // PD3 - Green LED
    
    // Test sequence - try ALL combinations
    while(1) {
        // Test 1: o3's correct logic - all LOW
        gpio_write(GPIOC_BASE, 6, 0);  // PC6 LOW = Power ON
        gpio_write(GPIOA_BASE, 3, 0);  // PA3 LOW = LCD ON
        gpio_write(GPIOB_BASE, 1, 0);  // PB1 LOW = Backlight enable
        gpio_write(GPIOB_BASE, 2, 1);  // PB2 HIGH = Bright
        gpio_write(GPIOA_BASE, 7, 1);  // Red LED ON
        gpio_write(GPIOD_BASE, 3, 0);  // Green LED OFF
        delay_ms(2000);
        
        // Test 2: 洪哥's logic - all HIGH
        gpio_write(GPIOC_BASE, 6, 1);  // PC6 HIGH
        gpio_write(GPIOA_BASE, 3, 1);  // PA3 HIGH
        gpio_write(GPIOB_BASE, 1, 1);  // PB1 HIGH
        gpio_write(GPIOB_BASE, 2, 1);  // PB2 HIGH
        gpio_write(GPIOA_BASE, 7, 0);  // Red LED OFF
        gpio_write(GPIOD_BASE, 3, 1);  // Green LED ON
        delay_ms(2000);
        
        // Test 3: Mixed - PC6 LOW, others HIGH
        gpio_write(GPIOC_BASE, 6, 0);  // PC6 LOW = Power should stay ON
        gpio_write(GPIOA_BASE, 3, 1);  // PA3 HIGH
        gpio_write(GPIOB_BASE, 1, 1);  // PB1 HIGH
        gpio_write(GPIOB_BASE, 2, 0);  // PB2 LOW
        // Toggle LEDs
        gpio_write(GPIOA_BASE, 7, 1);  // Red LED ON
        gpio_write(GPIOD_BASE, 3, 1);  // Green LED ON
        delay_ms(2000);
        
        // Test 4: Try PD0/PD1 as power
        gpio_write(GPIOD_BASE, 0, 0);  // PD0 LOW
        gpio_write(GPIOD_BASE, 1, 0);  // PD1 LOW
        delay_ms(1000);
        gpio_write(GPIOD_BASE, 0, 1);  // PD0 HIGH
        gpio_write(GPIOD_BASE, 1, 1);  // PD1 HIGH
        delay_ms(1000);
        
        // Test 5: Pulse PC6 to see if it needs edge trigger
        for(int i = 0; i < 5; i++) {
            gpio_write(GPIOC_BASE, 6, 0);  // LOW
            delay_ms(200);
            gpio_write(GPIOC_BASE, 6, 1);  // HIGH
            delay_ms(200);
        }
        gpio_write(GPIOC_BASE, 6, 0);  // End with LOW
        delay_ms(2000);
    }
}

// Minimal vector table
__attribute__((section(".isr_vector")))
const uint32_t g_pfnVectors[] = {
    (uint32_t)0x20002000,  // Initial stack pointer (8KB RAM end)
    (uint32_t)main,        // Reset handler
};