/* Complete version based on TIM10_Count with LED control */
/* This includes ALL necessary code to match the original 8KB size */

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

/* Memory mapped registers - WB32L003 */
#define FLASH_BASE            0x08000000
#define SRAM_BASE            0x20000000
#define PERIPH_BASE          0x40000000

/* RCC registers */
#define RCC_BASE             (PERIPH_BASE + 0x21000)
#define RCC_CR               (*(volatile uint32_t*)(RCC_BASE + 0x00))
#define RCC_CFGR             (*(volatile uint32_t*)(RCC_BASE + 0x04))
#define RCC_AHBENR           (*(volatile uint32_t*)(RCC_BASE + 0x14))
#define RCC_APB2ENR          (*(volatile uint32_t*)(RCC_BASE + 0x18))
#define RCC_APB1ENR          (*(volatile uint32_t*)(RCC_BASE + 0x1C))

/* GPIO registers */
#define GPIOA_BASE           (PERIPH_BASE + 0x8000000)
#define GPIOC_BASE           (PERIPH_BASE + 0x8000800)
#define GPIOD_BASE           (PERIPH_BASE + 0x8000C00)

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

#define GPIOA                ((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOC                ((GPIO_TypeDef*)GPIOC_BASE)
#define GPIOD                ((GPIO_TypeDef*)GPIOD_BASE)

/* TIM registers */
#define TIM1_BASE            (PERIPH_BASE + 0x12C00)
#define TIM10_BASE           (PERIPH_BASE + 0x14400)

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIM_TypeDef;

#define TIM1                 ((TIM_TypeDef*)TIM1_BASE)

/* BaseTim registers */
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CNT;
    volatile uint32_t ARR;
    volatile uint32_t SR;
} BASETIM_TypeDef;

#define TIM10                ((BASETIM_TypeDef*)TIM10_BASE)

/* NVIC */
#define NVIC_BASE            0xE000E100
#define NVIC_ISER            (*(volatile uint32_t*)(NVIC_BASE + 0x00))
#define NVIC_IPR0            (*(volatile uint32_t*)(NVIC_BASE + 0x300))

/* System functions - these will be linked from system_wb32l003.o */
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

/* Simple delay */
void delay(uint32_t count)
{
    while(count--) {
        volatile uint32_t i;
        for(i = 0; i < 100; i++);
    }
}

/* Initialize PC6 as 100kHz PWM - EXACTLY as original */
void Tim1_Config(void)
{
    /* Enable GPIOC clock */
    RCC_AHBENR |= (1 << 19);  /* IOPCEN */
    
    /* PC6 as AF1 (TIM1_CH1) */
    GPIOC->MODER &= ~(0x3 << 12);
    GPIOC->MODER |= (0x2 << 12);     /* Alternate function */
    GPIOC->OSPEEDR |= (0x3 << 12);   /* High speed */
    GPIOC->PUPDR &= ~(0x3 << 12);
    GPIOC->PUPDR |= (0x1 << 12);     /* Pull-up */
    GPIOC->AFR[0] &= ~(0xF << 24);
    GPIOC->AFR[0] |= (0x1 << 24);    /* AF1 */
    
    /* Enable TIM1 clock */
    RCC_APB2ENR |= (1 << 11);  /* TIM1EN */
    
    /* TIM1 configuration for 100kHz PWM */
    TIM1->PSC = 11;      /* Prescaler = 12-1 */
    TIM1->ARR = 9;       /* Period = 10-1 */
    TIM1->CCR1 = 5;      /* 50% duty cycle */
    TIM1->RCR = 0;       /* Repetition counter */
    
    /* PWM mode 1 configuration */
    TIM1->CCMR1 = 0x0068;  /* OC1M=110 (PWM1), OC1PE=1 */
    TIM1->CCER = 0x0001;   /* CC1E=1 */
    TIM1->BDTR = 0x8000;   /* MOE=1 */
    TIM1->CR1 = 0x0081;    /* CEN=1, ARPE=1 */
    TIM1->EGR = 0x0001;    /* Generate update event */
}

/* Configure BaseTim10 */
void BaseTim10_Config(void)
{
    /* Enable BASETIM clock */
    RCC_APB1ENR |= (1 << 5);  /* TIM10EN */
    
    /* Configure TIM10 */
    TIM10->CR = 0x00;      /* Reset control register */
    TIM10->ARR = 65435;    /* Auto-reload value */
    TIM10->CNT = 0;        /* Clear counter */
    
    /* Enable interrupt */
    TIM10->SR = 0;         /* Clear flags */
    TIM10->CR |= (1 << 1); /* Enable interrupt */
    TIM10->CR |= (1 << 0); /* Enable counter */
    
    /* Enable TIM10 interrupt in NVIC */
    NVIC_ISER |= (1 << 13);  /* TIM10 is IRQ 13 */
}

/* LED and GPIO configuration */
void GPIO_Config(void)
{
    /* Enable GPIOA, GPIOC, GPIOD clocks */
    RCC_AHBENR |= (1 << 17) | (1 << 19) | (1 << 20);
    
    /* PC0 - RED LED (output, low active) */
    GPIOC->MODER &= ~(0x3 << 0);
    GPIOC->MODER |= (0x1 << 0);      /* Output mode */
    GPIOC->OTYPER &= ~(1 << 0);      /* Push-pull */
    GPIOC->OSPEEDR |= (0x3 << 0);    /* High speed */
    GPIOC->PUPDR &= ~(0x3 << 0);     /* No pull */
    GPIOC->BSRR = (1 << 16);         /* Initially OFF (high) */
    
    /* PC1 - GREEN LED (output, low active) */
    GPIOC->MODER &= ~(0x3 << 2);
    GPIOC->MODER |= (0x1 << 2);      /* Output mode */
    GPIOC->OTYPER &= ~(1 << 1);      /* Push-pull */
    GPIOC->OSPEEDR |= (0x3 << 2);    /* High speed */
    GPIOC->PUPDR &= ~(0x3 << 2);     /* No pull */
    GPIOC->BSRR = (1 << 17);         /* Initially OFF (high) */
    
    /* PA3 - LCD Power (output, high active) */
    GPIOA->MODER &= ~(0x3 << 6);
    GPIOA->MODER |= (0x1 << 6);      /* Output mode */
    GPIOA->OSPEEDR |= (0x3 << 6);    /* High speed */
    GPIOA->PUPDR |= (0x1 << 6);      /* Pull-up */
    GPIOA->BSRR = (1 << 3);          /* Power ON */
    
    /* PC5 - RF Power (output, high active) */
    GPIOC->MODER &= ~(0x3 << 10);
    GPIOC->MODER |= (0x1 << 10);     /* Output mode */
    GPIOC->OSPEEDR |= (0x3 << 10);   /* High speed */
    GPIOC->PUPDR |= (0x1 << 10);     /* Pull-up */
    GPIOC->BSRR = (1 << 5);          /* Power ON */
    
    /* PD4 - TIM10 external input (AF6) - keep as original */
    GPIOD->MODER &= ~(0x3 << 8);
    GPIOD->MODER |= (0x2 << 8);      /* Alternate function */
    GPIOD->OSPEEDR |= (0x3 << 8);    /* High speed */
    GPIOD->PUPDR |= (0x1 << 8);      /* Pull-up */
    GPIOD->AFR[0] &= ~(0xF << 16);
    GPIOD->AFR[0] |= (0x6 << 16);    /* AF6 */
}

/* Main function */
int main(void)
{
    /* System initialization - CRITICAL */
    SystemInit();
    SystemCoreClockUpdate();
    
    /* Configure PC6 as 100kHz PWM - MUST BE FIRST */
    Tim1_Config();
    
    /* Configure GPIOs and LEDs */
    GPIO_Config();
    
    /* Configure BaseTim10 */
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
    static uint32_t toggle = 0;
    
    /* Check and clear interrupt flag */
    if(TIM10->SR & 0x01)
    {
        TIM10->SR = 0;  /* Clear flag */
        
        /* Toggle LEDs */
        toggle = !toggle;
        if(toggle)
        {
            GPIOC->BSRR = (1 << 0) | (1 << 17);   /* PC0 LOW (RED ON), PC1 HIGH (GREEN OFF) */
        }
        else
        {
            GPIOC->BSRR = (1 << 16) | (1 << 1);   /* PC0 HIGH (RED OFF), PC1 LOW (GREEN ON) */
        }
    }
}

/* Required handlers */
void NMI_Handler(void) { while(1); }
void HardFault_Handler(void) { while(1); }
void SVC_Handler(void) { while(1); }
void PendSV_Handler(void) { while(1); }
void SysTick_Handler(void) { while(1); }