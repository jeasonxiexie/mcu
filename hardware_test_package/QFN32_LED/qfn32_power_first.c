/* QFN32 Power-First Version - 先开电源再初始化 */

/* Base Addresses */
#define RCC_BASE        0x40021000
#define GPIOA_BASE      0x48000000  
#define GPIOC_BASE      0x48000800
#define TIM1_BASE       0x40012C00

/* RCC Registers */
#define RCC_AHBENR      (*(volatile unsigned int*)(RCC_BASE + 0x14))
#define RCC_APB2ENR     (*(volatile unsigned int*)(RCC_BASE + 0x18))

/* GPIO Registers */
#define GPIOA_MODER     (*(volatile unsigned int*)(GPIOA_BASE + 0x00))
#define GPIOA_OTYPER    (*(volatile unsigned int*)(GPIOA_BASE + 0x04))
#define GPIOA_OSPEEDR   (*(volatile unsigned int*)(GPIOA_BASE + 0x08))
#define GPIOA_PUPDR     (*(volatile unsigned int*)(GPIOA_BASE + 0x0C))
#define GPIOA_BSRR      (*(volatile unsigned int*)(GPIOA_BASE + 0x18))

#define GPIOC_MODER     (*(volatile unsigned int*)(GPIOC_BASE + 0x00))
#define GPIOC_OTYPER    (*(volatile unsigned int*)(GPIOC_BASE + 0x04))
#define GPIOC_OSPEEDR   (*(volatile unsigned int*)(GPIOC_BASE + 0x08))
#define GPIOC_PUPDR     (*(volatile unsigned int*)(GPIOC_BASE + 0x0C))
#define GPIOC_BSRR      (*(volatile unsigned int*)(GPIOC_BASE + 0x18))
#define GPIOC_AFR0      (*(volatile unsigned int*)(GPIOC_BASE + 0x20))

/* Simple delay function */
void delay(unsigned int count)
{
    volatile unsigned int i;
    while(count--) {
        for(i = 0; i < 1000; i++);
    }
}

/* Main function */
int main(void)
{
    /* CRITICAL: First enable all power supplies! */
    
    /* Enable GPIOA and GPIOC clocks */
    RCC_AHBENR |= (1 << 17) | (1 << 19);  /* IOPAEN | IOPCEN */
    
    /* Step 1: PC6 as GPIO OUTPUT HIGH first (CON_POW_CPU) */
    GPIOC_MODER &= ~(0x3 << 12);
    GPIOC_MODER |= (0x1 << 12);   /* Output mode */
    GPIOC_OSPEEDR |= (0x3 << 12); /* High speed */
    GPIOC_PUPDR &= ~(0x3 << 12);
    GPIOC_PUPDR |= (0x1 << 12);   /* Pull-up */
    GPIOC_BSRR = (1 << 6);        /* PC6 HIGH - CPU POWER ON! */
    
    /* Step 2: PA3 HIGH (CON_POW_LCD) */
    GPIOA_MODER &= ~(0x3 << 6);
    GPIOA_MODER |= (0x1 << 6);    /* Output mode */
    GPIOA_OSPEEDR |= (0x3 << 6);  /* High speed */
    GPIOA_PUPDR &= ~(0x3 << 6);
    GPIOA_PUPDR |= (0x1 << 6);    /* Pull-up */
    GPIOA_BSRR = (1 << 3);        /* PA3 HIGH - LCD POWER ON */
    
    /* Step 3: PC5 HIGH (CON_POW_RF) */
    GPIOC_MODER &= ~(0x3 << 10);
    GPIOC_MODER |= (0x1 << 10);   /* Output mode */
    GPIOC_OSPEEDR |= (0x3 << 10); /* High speed */
    GPIOC_PUPDR &= ~(0x3 << 10);
    GPIOC_PUPDR |= (0x1 << 10);   /* Pull-up */
    GPIOC_BSRR = (1 << 5);        /* PC5 HIGH - RF POWER ON */
    
    /* Wait for power stabilization */
    delay(100);
    
    /* Step 4: Setup LED pins */
    /* PC0 - RED LED (LOW active) */
    GPIOC_MODER &= ~(0x3 << 0);
    GPIOC_MODER |= (0x1 << 0);    /* Output mode */
    GPIOC_OSPEEDR |= (0x3 << 0);  /* High speed */
    GPIOC_OTYPER &= ~(1 << 0);    /* Push-pull */
    GPIOC_PUPDR &= ~(0x3 << 0);   /* No pull */
    
    /* PC1 - GREEN LED (LOW active) */
    GPIOC_MODER &= ~(0x3 << 2);
    GPIOC_MODER |= (0x1 << 2);    /* Output mode */
    GPIOC_OSPEEDR |= (0x3 << 2);  /* High speed */
    GPIOC_OTYPER &= ~(1 << 1);    /* Push-pull */
    GPIOC_PUPDR &= ~(0x3 << 2);   /* No pull */
    
    /* Initially turn off both LEDs */
    GPIOC_BSRR = (1 << 16) | (1 << 17);  /* PC0 HIGH, PC1 HIGH */
    
    /* Simple LED blink test */
    while(1)
    {
        /* Turn on RED LED only */
        GPIOC_BSRR = (1 << 0);     /* PC0 LOW - RED ON */
        delay(500);
        GPIOC_BSRR = (1 << 16);    /* PC0 HIGH - RED OFF */
        delay(500);
        
        /* Turn on GREEN LED only */
        GPIOC_BSRR = (1 << 1);     /* PC1 LOW - GREEN ON */
        delay(500);
        GPIOC_BSRR = (1 << 17);    /* PC1 HIGH - GREEN OFF */
        delay(500);
    }
}

/* Required for linking */
void SystemInit(void)
{
    /* System initialization if needed */
}