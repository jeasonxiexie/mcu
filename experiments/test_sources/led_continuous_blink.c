/* Continuous LED blink test to verify sustained operation */
typedef unsigned int uint32_t;

/* Direct register definitions */
#define RCC_AHBENR      (*(volatile uint32_t *)0x40021014)
#define GPIOA_MODER     (*(volatile uint32_t *)0x48000000)
#define GPIOA_ODR       (*(volatile uint32_t *)0x48000014)
#define GPIOB_MODER     (*(volatile uint32_t *)0x48000400)
#define GPIOB_ODR       (*(volatile uint32_t *)0x48000414)
#define GPIOC_MODER     (*(volatile uint32_t *)0x48000800)
#define GPIOC_ODR       (*(volatile uint32_t *)0x48000814)

/* Function prototypes */
void SystemInit(void);
void Early_PowerLatch(void);

/* Weak alias for libc */
void __libc_init_array(void) __attribute__((weak, alias("SystemInit")));

/* Simple delay */
void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}

/* Early power latch - must be first */
void Early_PowerLatch(void) {
    /* Enable GPIOC clock immediately */
    RCC_AHBENR |= (1 << 19);
    
    /* Configure PC6 as output */
    GPIOC_MODER &= ~(3 << 12);
    GPIOC_MODER |= (1 << 12);
    
    /* Set PC6 LOW to latch power ON */
    GPIOC_ODR &= ~(1 << 6);
    
    /* Small delay to ensure latch */
    delay(10000);
}

/* Empty system init */
void SystemInit(void) {
}

int main(void) {
    /* Ensure power latch first */
    Early_PowerLatch();
    
    /* Enable all GPIO clocks */
    RCC_AHBENR |= (1 << 17) | (1 << 18) | (1 << 19);
    
    /* Try multiple potential LED pins */
    /* PA1, PA2, PA3 as outputs */
    GPIOA_MODER &= ~((3 << 2) | (3 << 4) | (3 << 6));
    GPIOA_MODER |= (1 << 2) | (1 << 4) | (1 << 6);
    
    /* PB0, PB1, PB2 as outputs */
    GPIOB_MODER &= ~((3 << 0) | (3 << 2) | (3 << 4));
    GPIOB_MODER |= (1 << 0) | (1 << 2) | (1 << 4);
    
    /* PC7 as output (might be LED or backlight) */
    GPIOC_MODER &= ~(3 << 14);
    GPIOC_MODER |= (1 << 14);
    
    /* Main loop - toggle all potential LED pins */
    while(1) {
        /* Turn all potential LEDs ON */
        GPIOA_ODR |= (1 << 1) | (1 << 2) | (1 << 3);
        GPIOB_ODR |= (1 << 0) | (1 << 1) | (1 << 2);
        GPIOC_ODR |= (1 << 7);
        
        /* Delay */
        delay(2000000);
        
        /* Turn all potential LEDs OFF */
        GPIOA_ODR &= ~((1 << 1) | (1 << 2) | (1 << 3));
        GPIOB_ODR &= ~((1 << 0) | (1 << 1) | (1 << 2));
        GPIOC_ODR &= ~(1 << 7);
        
        /* Delay */
        delay(2000000);
    }
    
    return 0;
}