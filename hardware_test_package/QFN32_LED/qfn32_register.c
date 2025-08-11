/* QFN32 Pure Register Version - No Library Dependencies */

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

/* TIM1 Registers */
#define TIM1_CR1        (*(volatile unsigned int*)(TIM1_BASE + 0x00))
#define TIM1_CR2        (*(volatile unsigned int*)(TIM1_BASE + 0x04))
#define TIM1_CCMR1      (*(volatile unsigned int*)(TIM1_BASE + 0x18))
#define TIM1_CCER       (*(volatile unsigned int*)(TIM1_BASE + 0x20))
#define TIM1_PSC        (*(volatile unsigned int*)(TIM1_BASE + 0x28))
#define TIM1_ARR        (*(volatile unsigned int*)(TIM1_BASE + 0x2C))
#define TIM1_CCR1       (*(volatile unsigned int*)(TIM1_BASE + 0x34))
#define TIM1_BDTR       (*(volatile unsigned int*)(TIM1_BASE + 0x44))
#define TIM1_EGR        (*(volatile unsigned int*)(TIM1_BASE + 0x14))

/* Simple delay function */
void delay(unsigned int count)
{
    volatile unsigned int i;
    while(count--) {
        for(i = 0; i < 1000; i++);
    }
}

/* Setup PC6 as 100kHz PWM output - CRITICAL FOR SYSTEM */
void setup_pc6_pwm(void)
{
    /* Enable GPIOC clock */
    RCC_AHBENR |= (1 << 19);  /* IOPCEN */
    
    /* PC6 as AF1 (TIM1_CH1) */
    /* Clear PC6 mode bits */
    GPIOC_MODER &= ~(0x3 << 12);
    GPIOC_MODER |= (0x2 << 12);  /* Alternate function mode */
    
    /* Set PC6 to AF1 */
    GPIOC_AFR0 &= ~(0xF << 24);
    GPIOC_AFR0 |= (0x1 << 24);   /* AF1 for TIM1_CH1 */
    
    /* PC6 high speed, pull-up */
    GPIOC_OSPEEDR |= (0x3 << 12);  /* High speed */
    GPIOC_PUPDR &= ~(0x3 << 12);
    GPIOC_PUPDR |= (0x1 << 12);     /* Pull-up */
    
    /* Enable TIM1 clock */
    RCC_APB2ENR |= (1 << 11);  /* TIM1EN */
    
    /* Configure TIM1 for 100kHz PWM */
    /* System clock = 12MHz after reset */
    /* 100kHz = 12MHz / (PSC+1) / (ARR+1) */
    /* 100kHz = 12MHz / 12 / 10 = 100kHz */
    
    TIM1_PSC = 11;   /* Prescaler = 12-1 */
    TIM1_ARR = 9;    /* Auto-reload = 10-1 */
    TIM1_CCR1 = 5;   /* 50% duty cycle */
    
    /* Configure Channel 1 as PWM mode 1 */
    TIM1_CCMR1 = 0x0068;  /* OC1M=110 (PWM mode 1), OC1PE=1 */
    
    /* Enable Channel 1 output */
    TIM1_CCER = 0x0001;   /* CC1E=1 */
    
    /* Enable main output */
    TIM1_BDTR = 0x8000;   /* MOE=1 */
    
    /* Update generation */
    TIM1_EGR = 0x0001;    /* UG=1 */
    
    /* Enable counter with auto-reload preload */
    TIM1_CR1 = 0x0081;    /* CEN=1, ARPE=1 */
}

/* Setup power control and LEDs */
void setup_gpio(void)
{
    /* Enable GPIOA clock */
    RCC_AHBENR |= (1 << 17);  /* IOPAEN */
    
    /* PA3 as output (LCD Power) - HIGH active */
    GPIOA_MODER &= ~(0x3 << 6);
    GPIOA_MODER |= (0x1 << 6);    /* Output mode */
    GPIOA_OSPEEDR |= (0x3 << 6);  /* High speed */
    GPIOA_PUPDR &= ~(0x3 << 6);
    GPIOA_PUPDR |= (0x1 << 6);    /* Pull-up */
    GPIOA_BSRR = (1 << 3);        /* Set PA3 HIGH */
    
    /* PC5 as output (RF Power) - HIGH active */
    GPIOC_MODER &= ~(0x3 << 10);
    GPIOC_MODER |= (0x1 << 10);   /* Output mode */
    GPIOC_OSPEEDR |= (0x3 << 10); /* High speed */
    GPIOC_PUPDR &= ~(0x3 << 10);
    GPIOC_PUPDR |= (0x1 << 10);   /* Pull-up */
    GPIOC_BSRR = (1 << 5);        /* Set PC5 HIGH */
    
    /* PC0 as output (RED LED) - LOW active */
    GPIOC_MODER &= ~(0x3 << 0);
    GPIOC_MODER |= (0x1 << 0);    /* Output mode */
    GPIOC_OSPEEDR |= (0x3 << 0);  /* High speed */
    GPIOC_PUPDR &= ~(0x3 << 0);   /* No pull */
    GPIOC_BSRR = (1 << 16);       /* Set PC0 HIGH (LED OFF) */
    
    /* PC1 as output (GREEN LED) - LOW active */
    GPIOC_MODER &= ~(0x3 << 2);
    GPIOC_MODER |= (0x1 << 2);    /* Output mode */
    GPIOC_OSPEEDR |= (0x3 << 2);  /* High speed */
    GPIOC_PUPDR &= ~(0x3 << 2);   /* No pull */
    GPIOC_BSRR = (1 << 17);       /* Set PC1 HIGH (LED OFF) */
}

/* Main function */
int main(void)
{
    /* Setup PC6 100kHz PWM - MUST BE FIRST */
    setup_pc6_pwm();
    
    /* Setup power and LEDs */
    setup_gpio();
    
    /* Main loop - alternate LEDs */
    while(1)
    {
        /* RED LED ON, GREEN LED OFF */
        GPIOC_BSRR = (1 << 0) | (1 << 17);  /* PC0 LOW, PC1 HIGH */
        delay(500);
        
        /* RED LED OFF, GREEN LED ON */
        GPIOC_BSRR = (1 << 16) | (1 << 1);  /* PC0 HIGH, PC1 LOW */
        delay(500);
    }
}

/* Required for linking */
void SystemInit(void)
{
    /* System initialization if needed */
}