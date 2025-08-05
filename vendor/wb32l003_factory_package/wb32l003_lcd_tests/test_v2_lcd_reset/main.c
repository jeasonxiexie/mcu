/**
 * Test v2: Backlight + LCD Reset
 * Test backlight and LCD reset sequence
 */

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

/* Direct register definitions */
#define RCC_AHBENR      (*(volatile uint32_t *)0x40021014)
#define GPIOB_MODER     (*(volatile uint32_t *)0x48000400)
#define GPIOB_ODR       (*(volatile uint32_t *)0x48000414)
#define GPIOC_MODER     (*(volatile uint32_t *)0x48000800)
#define GPIOC_ODR       (*(volatile uint32_t *)0x48000814)

/* LCD pins - Corrected to GPIOC based on schematic */
#define LCD_SCL_PIN     0   /* PC0 - Clock */
#define LCD_SDA_PIN     1   /* PC1 - Data/MOSI */
#define LCD_RST_PIN     3   /* PC3 - LCD Reset */
#define LCD_RS_PIN      4   /* PC4 - RS/DC */
#define LCD_CS_PIN      6   /* PC6 - Chip Select */

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

/* LED blink pattern */
void blink_led(int times) {
    for(int i = 0; i < times; i++) {
        GPIOB_ODR &= ~(1 << LED_RED_PIN);  /* ON */
        delay_ms(200);
        GPIOB_ODR |= (1 << LED_RED_PIN);   /* OFF */
        delay_ms(200);
    }
}

int main(void) {
    /* Power latch first */
    Early_PowerLatch();
    
    /* Configure GPIOB pins for LED and backlight */
    /* PB0: LED, PB1: BL1, PB2: BL2 */
    GPIOB_MODER &= ~((3 << 0) | (3 << 2) | (3 << 4));
    GPIOB_MODER |= (1 << 0) | (1 << 2) | (1 << 4);
    
    /* Configure GPIOC pins for LCD */
    /* PC0: SCL, PC1: SDA, PC3: RST, PC4: RS, PC6: CS */
    GPIOC_MODER &= ~((3 << 0) | (3 << 2) | (3 << 6) | (3 << 8) | (3 << 12));
    GPIOC_MODER |= (1 << 0) | (1 << 2) | (1 << 6) | (1 << 8) | (1 << 12);
    
    /* Initial states for GPIOB */
    GPIOB_ODR = (1 << LED_RED_PIN) |    /* LED OFF */
                (1 << BL1_PIN);          /* BL1 OFF */
    
    /* Initial states for GPIOC (LCD) */
    GPIOC_ODR = (1 << LCD_CS_PIN) |     /* CS HIGH (inactive) */
                (1 << LCD_RST_PIN);      /* RST HIGH initially */
    
    /* Turn on low brightness backlight */
    GPIOB_ODR |= (1 << BL2_PIN);  /* PB2 HIGH = Low brightness ON */
    
    /* Blink once to show start */
    blink_led(1);
    delay_ms(500);
    
    /* LCD Reset sequence */
    GPIOC_ODR |= (1 << LCD_RST_PIN);   /* RST HIGH */
    delay_ms(10);
    GPIOC_ODR &= ~(1 << LCD_RST_PIN);  /* RST LOW */
    delay_ms(10);
    GPIOC_ODR |= (1 << LCD_RST_PIN);   /* RST HIGH */
    delay_ms(120);
    
    /* Blink twice to show reset done */
    blink_led(2);
    delay_ms(500);
    
    /* Try high brightness */
    GPIOB_ODR &= ~(1 << BL1_PIN);  /* PB1 LOW = High brightness ON */
    
    /* Blink 3 times to show high brightness active */
    blink_led(3);
    
    /* Main loop - slow blink to show running */
    while(1) {
        /* Toggle between high and low brightness */
        GPIOB_ODR ^= (1 << BL1_PIN);
        
        /* LED indicates brightness state */
        if(GPIOB_ODR & (1 << BL1_PIN)) {
            /* Low brightness - slow blink */
            GPIOB_ODR &= ~(1 << LED_RED_PIN);
            delay_ms(1000);
            GPIOB_ODR |= (1 << LED_RED_PIN);
            delay_ms(1000);
        } else {
            /* High brightness - fast blink */
            GPIOB_ODR &= ~(1 << LED_RED_PIN);
            delay_ms(200);
            GPIOB_ODR |= (1 << LED_RED_PIN);
            delay_ms(200);
        }
    }
    
    return 0;
}