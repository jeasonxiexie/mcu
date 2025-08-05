/**
 * @file power_high_minimal.c
 * @brief Test with PC6 HIGH instead of LOW
 */

/* Register definitions */
#define RCC_AHBENR  (*(volatile unsigned int*)0x40021014)
#define GPIOC_MODER (*(volatile unsigned int*)0x48000800)
#define GPIOC_ODR   (*(volatile unsigned int*)0x48000814)

/* Main function - busy loop */
int main(void)
{
    volatile unsigned int i = 0;
    
    /* Enable GPIOC clock */
    RCC_AHBENR |= (1 << 19);
    
    /* Configure PC6 as output */
    GPIOC_MODER &= ~(3 << 12);
    GPIOC_MODER |= (1 << 12);
    
    /* Set PC6 HIGH (opposite of before!) */
    GPIOC_ODR |= (1 << 6);
    
    /* Busy loop to generate heat */
    while(1)
    {
        i++;
        if(i > 1000000) i = 0;
    }
}

/* Power latch with PC6 HIGH */
void Early_PowerLatch(void)
{
    /* Enable GPIOC clock */
    RCC_AHBENR |= (1 << 19);
    
    /* Configure PC6 as output */
    GPIOC_MODER &= ~(3 << 12);
    GPIOC_MODER |= (1 << 12);
    
    /* Set PC6 HIGH instead of LOW! */
    GPIOC_ODR |= (1 << 6);
}

/* Empty stubs */
void SystemInit(void) {}
void __libc_init_array(void) {}