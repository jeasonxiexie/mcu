/**
 * Test v1: Backlight Only
 * Just test if we can turn on the backlight
 */

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

/* Direct register definitions */
#define RCC_AHBENR      (*(volatile uint32_t *)0x40021014)
#define GPIOB_MODER     (*(volatile uint32_t *)0x48000400)
#define GPIOB_ODR       (*(volatile uint32_t *)0x48000414)
#define GPIOC_MODER     (*(volatile uint32_t *)0x48000800)
#define GPIOC_ODR       (*(volatile uint32_t *)0x48000814)

/* Backlight control pins */
#define BL1_PIN         1   /* PB1 - High brightness (LOW = ON) */
#define BL2_PIN         2   /* PB2 - Low brightness (HIGH = ON) */

/* LED control */
#define LED_RED_PIN     0   /* PB0 - Red LED (LOW = ON) */

/* Power control */
#define POWER_PIN       6   /* PC6 - Power latch (HIGH = Keep power) */

/* Required functions */
void Early_PowerLatch(void);
void SystemInit(void);
void __libc_init_array(void);

void Early_PowerLatch(void) {
    /* Enable GPIO clocks */
    RCC_AHBENR |= (1 << 18) | (1 << 19);  /* GPIOB, GPIOC */
    
    /* Configure PC6 as output for power latch */
    GPIOC_MODER &= ~(3 << 12);
    GPIOC_MODER |= (1 << 12);
    
    /* Set PC6 HIGH to keep power */
    GPIOC_ODR |= (1 << POWER_PIN);
}

void SystemInit(void) {}
void __libc_init_array(void) {}

/* Simple delay */
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        for(volatile uint32_t j = 0; j < 2400; j++);
    }
}

int main(void) {
    /* Power latch first */
    Early_PowerLatch();
    
    /* Configure backlight and LED pins as outputs */
    /* PB0 (LED), PB1 (BL1), PB2 (BL2) */
    GPIOB_MODER &= ~((3 << 0) | (3 << 2) | (3 << 4));
    GPIOB_MODER |= (1 << 0) | (1 << 2) | (1 << 4);
    
    /* Turn off LED initially */
    GPIOB_ODR |= (1 << LED_RED_PIN);  /* HIGH = OFF */
    
    /* Turn on backlight - try low brightness first (safer) */
    GPIOB_ODR |= (1 << BL2_PIN);      /* PB2 HIGH = Low brightness ON */
    GPIOB_ODR |= (1 << BL1_PIN);      /* PB1 HIGH = High brightness OFF */
    
    /* Blink LED slowly to show program is running */
    while(1) {
        /* LED ON */
        GPIOB_ODR &= ~(1 << LED_RED_PIN);
        delay_ms(500);
        
        /* LED OFF */
        GPIOB_ODR |= (1 << LED_RED_PIN);
        delay_ms(500);
        
        /* After 5 blinks, try high brightness */
        static int count = 0;
        count++;
        if(count == 5) {
            /* Switch to high brightness */
            GPIOB_ODR &= ~(1 << BL1_PIN);  /* PB1 LOW = High brightness ON */
            delay_ms(2000);
            
            /* Back to low brightness */
            GPIOB_ODR |= (1 << BL1_PIN);   /* PB1 HIGH = High brightness OFF */
            count = 0;
        }
    }
    
    return 0;
}