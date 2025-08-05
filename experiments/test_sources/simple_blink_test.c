/**
 * @file simple_blink_test.c
 * @brief Simple LED blink test with early power latch
 */

/* Register definitions */
#define RCC_AHBENR  (*(volatile unsigned int*)0x40021014)
#define GPIOC_MODER (*(volatile unsigned int*)0x48000800)
#define GPIOC_ODR   (*(volatile unsigned int*)0x48000814)
#define GPIOB_MODER (*(volatile unsigned int*)0x48000400)
#define GPIOB_ODR   (*(volatile unsigned int*)0x48000414)

/* Simple delay */
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 4800; j++) { /* ~1ms at 48MHz */
            __asm__("nop");
        }
    }
}

/* Early power latch - called before main */
void Early_PowerLatch(void)
{
    /* Enable GPIOC clock */
    RCC_AHBENR |= (1 << 19);
    
    /* Configure PC6 as output */
    GPIOC_MODER &= ~(3 << 12);
    GPIOC_MODER |= (1 << 12);
    
    /* Set PC6 LOW to latch power */
    GPIOC_ODR &= ~(1 << 6);
}

/* Main function */
int main(void)
{
    /* Enable GPIOB clock for LED test */
    RCC_AHBENR |= (1 << 18);
    
    /* Configure PB0 and PB1 as outputs (LED pins) */
    GPIOB_MODER &= ~(3 << 0);
    GPIOB_MODER |= (1 << 0);
    GPIOB_MODER &= ~(3 << 2);
    GPIOB_MODER |= (1 << 2);
    
    /* Blink pattern to indicate program is running */
    while(1)
    {
        /* Turn on PB0 */
        GPIOB_ODR |= (1 << 0);
        delay_ms(500);
        
        /* Turn off PB0 */
        GPIOB_ODR &= ~(1 << 0);
        delay_ms(500);
        
        /* Toggle PB1 */
        GPIOB_ODR ^= (1 << 1);
    }
}

/* Empty stubs */
void SystemInit(void) {}
void __libc_init_array(void) {}