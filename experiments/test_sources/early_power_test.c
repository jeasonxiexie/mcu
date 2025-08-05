/**
 * @file early_power_test.c
 * @brief Test early power latch within 150ms window
 */

/* Register definitions */
#define RCC_AHBENR  (*(volatile unsigned int*)0x40021014)
#define GPIOC_MODER (*(volatile unsigned int*)0x48000800)
#define GPIOC_ODR   (*(volatile unsigned int*)0x48000814)
#define GPIOC_IDR   (*(volatile unsigned int*)0x48000810)

/* Very early power latch - runs immediately */
void Early_PowerLatch(void) __attribute__((section(".text.early_power")));
void Early_PowerLatch(void)
{
    /* Enable GPIOC clock IMMEDIATELY */
    RCC_AHBENR |= (1 << 19);
    
    /* Small delay for clock to stabilize */
    for(volatile int i = 0; i < 10; i++);
    
    /* Configure PC6 as output */
    GPIOC_MODER &= ~(3 << 12);
    GPIOC_MODER |= (1 << 12);
    
    /* Set PC6 LOW to latch power - CRITICAL! */
    GPIOC_ODR &= ~(1 << 6);
    
    /* Also configure PC7 as input for power button */
    GPIOC_MODER &= ~(3 << 14);
}

/* Main function */
int main(void)
{
    volatile unsigned int counter = 0;
    
    /* Main loop - generate heat */
    while(1)
    {
        counter++;
        
        /* Check power button (PC7) */
        if(GPIOC_IDR & (1 << 7))
        {
            /* Button pressed - do some work */
            for(volatile int i = 0; i < 100000; i++);
        }
        
        /* Keep PC6 LOW */
        if((counter & 0xFFFF) == 0)
        {
            GPIOC_ODR &= ~(1 << 6);
        }
    }
}

/* SystemInit runs very early, before main */
void SystemInit(void)
{
    /* Ensure PC6 stays LOW */
    GPIOC_ODR &= ~(1 << 6);
}

/* Empty stub */
void __libc_init_array(void) {}