/* Full standalone version with complete initialization */
/* Based on TIM10_Count but self-contained */

/* Base addresses */
#define FLASH_BASE      0x00000000
#define SRAM_BASE       0x20000000
#define PERIPH_BASE     0x40000000

/* System Control */
#define SCB_BASE        0xE000ED00
#define SCB_VTOR        (*(volatile unsigned int*)(SCB_BASE + 0x08))
#define SCB_AIRCR       (*(volatile unsigned int*)(SCB_BASE + 0x0C))
#define SCB_SCR         (*(volatile unsigned int*)(SCB_BASE + 0x10))

/* RCC */
#define RCC_BASE        0x40021000
#define RCC_CR          (*(volatile unsigned int*)(RCC_BASE + 0x00))
#define RCC_CFGR        (*(volatile unsigned int*)(RCC_BASE + 0x04))
#define RCC_AHBENR      (*(volatile unsigned int*)(RCC_BASE + 0x14))
#define RCC_APB2ENR     (*(volatile unsigned int*)(RCC_BASE + 0x18))
#define RCC_APB1ENR     (*(volatile unsigned int*)(RCC_BASE + 0x1C))

/* GPIO */
#define GPIOA_BASE      0x48000000
#define GPIOC_BASE      0x48000800
#define GPIOD_BASE      0x48000C00

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

#define GPIOD_MODER     (*(volatile unsigned int*)(GPIOD_BASE + 0x00))
#define GPIOD_OSPEEDR   (*(volatile unsigned int*)(GPIOD_BASE + 0x08))
#define GPIOD_PUPDR     (*(volatile unsigned int*)(GPIOD_BASE + 0x0C))
#define GPIOD_AFR0      (*(volatile unsigned int*)(GPIOD_BASE + 0x20))

/* TIM1 */
#define TIM1_BASE       0x40012C00
#define TIM1_CR1        (*(volatile unsigned int*)(TIM1_BASE + 0x00))
#define TIM1_CR2        (*(volatile unsigned int*)(TIM1_BASE + 0x04))
#define TIM1_DIER       (*(volatile unsigned int*)(TIM1_BASE + 0x0C))
#define TIM1_SR         (*(volatile unsigned int*)(TIM1_BASE + 0x10))
#define TIM1_EGR        (*(volatile unsigned int*)(TIM1_BASE + 0x14))
#define TIM1_CCMR1      (*(volatile unsigned int*)(TIM1_BASE + 0x18))
#define TIM1_CCER       (*(volatile unsigned int*)(TIM1_BASE + 0x20))
#define TIM1_CNT        (*(volatile unsigned int*)(TIM1_BASE + 0x24))
#define TIM1_PSC        (*(volatile unsigned int*)(TIM1_BASE + 0x28))
#define TIM1_ARR        (*(volatile unsigned int*)(TIM1_BASE + 0x2C))
#define TIM1_RCR        (*(volatile unsigned int*)(TIM1_BASE + 0x30))
#define TIM1_CCR1       (*(volatile unsigned int*)(TIM1_BASE + 0x34))
#define TIM1_BDTR       (*(volatile unsigned int*)(TIM1_BASE + 0x44))

/* TIM10 (BaseTim) */
#define TIM10_BASE      0x40014400
#define TIM10_CR        (*(volatile unsigned int*)(TIM10_BASE + 0x00))
#define TIM10_CNT       (*(volatile unsigned int*)(TIM10_BASE + 0x04))
#define TIM10_ARR       (*(volatile unsigned int*)(TIM10_BASE + 0x08))
#define TIM10_SR        (*(volatile unsigned int*)(TIM10_BASE + 0x10))

/* NVIC */
#define NVIC_ISER       (*(volatile unsigned int*)0xE000E100)
#define NVIC_IPR0       (*(volatile unsigned int*)0xE000E400)
#define NVIC_IPR1       (*(volatile unsigned int*)0xE000E404)
#define NVIC_IPR2       (*(volatile unsigned int*)0xE000E408)
#define NVIC_IPR3       (*(volatile unsigned int*)0xE000E40C)

/* System initialization like the original */
void SystemInit(void)
{
    /* Enable HSI and wait for it to be ready */
    RCC_CR |= 0x00000001;  /* HSION */
    while(!(RCC_CR & 0x00000002));  /* Wait for HSIRDY */
    
    /* Configure Flash latency */
    volatile unsigned int *FLASH_ACR = (volatile unsigned int*)0x40022000;
    *FLASH_ACR = 0x00000000;  /* 0 wait states for 12MHz */
    
    /* Configure system clock */
    RCC_CFGR = 0x00000000;  /* HSI as system clock */
    
    /* Enable prefetch buffer */
    *FLASH_ACR |= 0x00000010;
}

/* Clock update function */
void SystemCoreClockUpdate(void)
{
    /* System runs at 12MHz from HSI */
    /* This matches the original configuration */
}

/* Delay function */
void delay(unsigned int count)
{
    volatile unsigned int i, j;
    for(i = 0; i < count; i++) {
        for(j = 0; j < 1000; j++);
    }
}

/* Configure TIM1 for 100kHz PWM on PC6 - EXACTLY as original */
void Tim1_Config(void)
{
    /* Enable GPIOC clock */
    RCC_AHBENR |= (1 << 19);  /* IOPCEN */
    
    /* PC6 as AF1 (TIM1_CH1) */
    GPIOC_MODER &= ~(0x3 << 12);
    GPIOC_MODER |= (0x2 << 12);     /* Alternate function mode */
    GPIOC_OSPEEDR |= (0x3 << 12);   /* High speed */
    GPIOC_PUPDR &= ~(0x3 << 12);
    GPIOC_PUPDR |= (0x1 << 12);     /* Pull-up */
    GPIOC_AFR0 &= ~(0xF << 24);
    GPIOC_AFR0 |= (0x1 << 24);      /* AF1 */
    
    /* Enable TIM1 clock */
    RCC_APB2ENR |= (1 << 11);  /* TIM1EN */
    
    /* Disable auto-reload preload initially */
    TIM1_CR1 &= ~(1 << 7);
    
    /* Configure TIM1 for 100kHz PWM */
    /* System clock = 12MHz, 100kHz = 12MHz / (PSC+1) / (ARR+1) */
    /* 100kHz = 12MHz / 12 / 10 */
    TIM1_PSC = 11;   /* Prescaler = 12-1 */
    TIM1_ARR = 9;    /* Period = 10-1 */
    TIM1_RCR = 0;    /* No repetition */
    
    /* Enable auto-reload preload */
    TIM1_CR1 |= (1 << 7);  /* ARPE */
    
    /* Configure PWM mode 1 on channel 1 */
    TIM1_CCMR1 = 0x0068;  /* OC1M=110 (PWM mode 1), OC1PE=1 */
    TIM1_CCR1 = 5;        /* 50% duty cycle */
    TIM1_CCER = 0x0001;   /* CC1E=1, enable channel 1 output */
    
    /* Enable preload control */
    TIM1_CR2 |= (1 << 0);  /* CCPC */
    
    /* Enable counter */
    TIM1_CR1 |= (1 << 0);  /* CEN */
    
    /* Enable main output */
    TIM1_BDTR |= (1 << 15);  /* MOE */
}

/* Configure BaseTim10 */
void BaseTim10_Config(void)
{
    /* Enable BASETIM clock */
    RCC_APB1ENR |= (1 << 5);  /* TIM10EN */
    
    /* Reset TIM10 */
    TIM10_CR = 0;
    TIM10_SR = 0;
    
    /* Configure TIM10 */
    TIM10_ARR = 65435;  /* Auto-reload value - same as original */
    TIM10_CNT = 0;      /* Clear counter */
    
    /* No gate, no toggle, count mode, auto-reload, 16-bit, repeat mode, DIV1 */
    /* This matches the original BaseTim configuration */
    
    /* Enable interrupt */
    TIM10_CR |= (1 << 1);  /* UIE - Update interrupt enable */
    
    /* Enable TIM10 interrupt in NVIC */
    /* TIM10 is interrupt #13 in the vector table */
    NVIC_ISER |= (1 << 13);
    
    /* Set priority (optional) */
    NVIC_IPR3 = (NVIC_IPR3 & ~(0xFF << 8)) | (1 << 14);  /* Priority 1 */
    
    /* Enable counter */
    TIM10_CR |= (1 << 0);  /* CEN */
}

/* Main function */
int main(void)
{
    /* System initialization - CRITICAL */
    SystemInit();
    SystemCoreClockUpdate();
    
    /* Configure PC6 as 100kHz PWM - MUST KEEP THIS */
    Tim1_Config();
    
    /* Enable GPIO clocks */
    RCC_AHBENR |= (1 << 17) | (1 << 19) | (1 << 20);  /* GPIOA, GPIOC, GPIOD */
    
    /* Configure PC0 - RED LED (low active) */
    GPIOC_MODER &= ~(0x3 << 0);
    GPIOC_MODER |= (0x1 << 0);      /* Output mode */
    GPIOC_OTYPER &= ~(1 << 0);      /* Push-pull */
    GPIOC_OSPEEDR |= (0x3 << 0);    /* High speed */
    GPIOC_PUPDR &= ~(0x3 << 0);     /* No pull */
    GPIOC_BSRR = (1 << 16);         /* Initially OFF */
    
    /* Configure PC1 - GREEN LED (low active) */
    GPIOC_MODER &= ~(0x3 << 2);
    GPIOC_MODER |= (0x1 << 2);      /* Output mode */
    GPIOC_OTYPER &= ~(1 << 1);      /* Push-pull */
    GPIOC_OSPEEDR |= (0x3 << 2);    /* High speed */
    GPIOC_PUPDR &= ~(0x3 << 2);     /* No pull */
    GPIOC_BSRR = (1 << 17);         /* Initially OFF */
    
    /* Configure PA3 - LCD Power */
    GPIOA_MODER &= ~(0x3 << 6);
    GPIOA_MODER |= (0x1 << 6);      /* Output mode */
    GPIOA_OSPEEDR |= (0x3 << 6);    /* High speed */
    GPIOA_PUPDR |= (0x1 << 6);      /* Pull-up */
    GPIOA_BSRR = (1 << 3);          /* Power ON */
    
    /* Configure PC5 - RF Power */
    GPIOC_MODER &= ~(0x3 << 10);
    GPIOC_MODER |= (0x1 << 10);     /* Output mode */
    GPIOC_OSPEEDR |= (0x3 << 10);   /* High speed */
    GPIOC_PUPDR |= (0x1 << 10);     /* Pull-up */
    GPIOC_BSRR = (1 << 5);          /* Power ON */
    
    /* Configure PD4 - TIM10 external input (AF6) */
    GPIOD_MODER &= ~(0x3 << 8);
    GPIOD_MODER |= (0x2 << 8);      /* Alternate function */
    GPIOD_OSPEEDR |= (0x3 << 8);    /* High speed */
    GPIOD_PUPDR |= (0x1 << 8);      /* Pull-up */
    GPIOD_AFR0 &= ~(0xF << 16);
    GPIOD_AFR0 |= (0x6 << 16);      /* AF6 */
    
    /* Configure and start BaseTim10 */
    BaseTim10_Config();
    
    /* Main loop */
    while(1)
    {
        /* Everything happens in interrupt */
    }
}

/* TIM10 interrupt handler */
void TIM10_IRQHandler(void)
{
    static unsigned int toggle = 0;
    
    /* Check interrupt flag */
    if(TIM10_SR & 0x01)
    {
        /* Clear interrupt flag */
        TIM10_SR = 0;
        
        /* Toggle LEDs */
        toggle = !toggle;
        if(toggle)
        {
            /* RED ON, GREEN OFF */
            GPIOC_BSRR = (1 << 0) | (1 << 17);
        }
        else
        {
            /* RED OFF, GREEN ON */
            GPIOC_BSRR = (1 << 16) | (1 << 1);
        }
    }
}