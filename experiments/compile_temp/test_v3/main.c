/**
 * Test v3: Full LCD Display Test
 * Complete LCD initialization and color display test
 * Based on corrected hardware analysis
 */

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

/* Direct register definitions */
#define RCC_AHBENR      (*(volatile uint32_t *)0x40021014)
#define GPIOB_MODER     (*(volatile uint32_t *)0x48000400)
#define GPIOB_ODR       (*(volatile uint32_t *)0x48000414)
#define GPIOC_MODER     (*(volatile uint32_t *)0x48000800)
#define GPIOC_ODR       (*(volatile uint32_t *)0x48000814)

/* LCD pins - ALL on GPIOB */
#define LCD_RST_PIN     7   /* PB7 - LCD Reset */
#define LCD_RS_PIN      6   /* PB6 - RS/DC */
#define LCD_SCK_PIN     5   /* PB5 - Clock */
#define LCD_CS_PIN      4   /* PB4 - Chip Select */
#define LCD_SDA_PIN     3   /* PB3 - Data */

/* Backlight control pins */
#define BL1_PIN         1   /* PB1 - High brightness (LOW = ON) */
#define BL2_PIN         2   /* PB2 - Low brightness (HIGH = ON) */

/* LED control */
#define LED_RED_PIN     0   /* PB0 - Red LED (LOW = ON) */

/* Power control */
#define POWER_PIN       6   /* PC6 - Power latch (HIGH = Keep power) */

/* LCD dimensions - 0.96" TFT standard */
#define LCD_WIDTH       80
#define LCD_HEIGHT      160

/* ST7735S Commands */
#define ST7735_NOP      0x00
#define ST7735_SWRESET  0x01
#define ST7735_SLPOUT   0x11
#define ST7735_NORON    0x13
#define ST7735_INVOFF   0x20
#define ST7735_DISPON   0x29
#define ST7735_CASET    0x2A
#define ST7735_RASET    0x2B
#define ST7735_RAMWR    0x2C
#define ST7735_MADCTL   0x36
#define ST7735_COLMOD   0x3A
#define ST7735_FRMCTR1  0xB1
#define ST7735_FRMCTR2  0xB2
#define ST7735_FRMCTR3  0xB3
#define ST7735_INVCTR   0xB4
#define ST7735_PWCTR1   0xC0
#define ST7735_PWCTR2   0xC1
#define ST7735_PWCTR3   0xC2
#define ST7735_PWCTR4   0xC3
#define ST7735_PWCTR5   0xC4
#define ST7735_VMCTR1   0xC5
#define ST7735_GMCTRP1  0xE0
#define ST7735_GMCTRN1  0xE1

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

/* LCD control macros */
#define LCD_CS_LOW()    (GPIOB_ODR &= ~(1 << LCD_CS_PIN))
#define LCD_CS_HIGH()   (GPIOB_ODR |= (1 << LCD_CS_PIN))
#define LCD_DC_LOW()    (GPIOB_ODR &= ~(1 << LCD_RS_PIN))
#define LCD_DC_HIGH()   (GPIOB_ODR |= (1 << LCD_RS_PIN))
#define LCD_SCK_LOW()   (GPIOB_ODR &= ~(1 << LCD_SCK_PIN))
#define LCD_SCK_HIGH()  (GPIOB_ODR |= (1 << LCD_SCK_PIN))
#define LCD_SDA_LOW()   (GPIOB_ODR &= ~(1 << LCD_SDA_PIN))
#define LCD_SDA_HIGH()  (GPIOB_ODR |= (1 << LCD_SDA_PIN))
#define LCD_RST_LOW()   (GPIOB_ODR &= ~(1 << LCD_RST_PIN))
#define LCD_RST_HIGH()  (GPIOB_ODR |= (1 << LCD_RST_PIN))

/* SPI write byte */
void spi_write_byte(uint8_t data) {
    for(int i = 7; i >= 0; i--) {
        LCD_SCK_LOW();
        if(data & (1 << i)) {
            LCD_SDA_HIGH();
        } else {
            LCD_SDA_LOW();
        }
        LCD_SCK_HIGH();
    }
}

/* Write command to LCD */
void lcd_write_cmd(uint8_t cmd) {
    LCD_DC_LOW();
    LCD_CS_LOW();
    spi_write_byte(cmd);
    LCD_CS_HIGH();
}

/* Write data to LCD */
void lcd_write_data(uint8_t data) {
    LCD_DC_HIGH();
    LCD_CS_LOW();
    spi_write_byte(data);
    LCD_CS_HIGH();
}

/* Set address window */
void lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    lcd_write_cmd(ST7735_CASET);
    lcd_write_data(x0 >> 8);
    lcd_write_data(x0 & 0xFF);
    lcd_write_data(x1 >> 8);
    lcd_write_data(x1 & 0xFF);
    
    lcd_write_cmd(ST7735_RASET);
    lcd_write_data(y0 >> 8);
    lcd_write_data(y0 & 0xFF);
    lcd_write_data(y1 >> 8);
    lcd_write_data(y1 & 0xFF);
    
    lcd_write_cmd(ST7735_RAMWR);
}

/* Fill screen with color */
void lcd_fill_color(uint16_t color) {
    lcd_set_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    
    uint8_t high = color >> 8;
    uint8_t low = color & 0xFF;
    
    LCD_DC_HIGH();
    LCD_CS_LOW();
    
    for(uint32_t i = 0; i < (uint32_t)LCD_WIDTH * LCD_HEIGHT; i++) {
        spi_write_byte(high);
        spi_write_byte(low);
    }
    
    LCD_CS_HIGH();
}

/* Initialize LCD */
void lcd_init(void) {
    /* Hardware reset */
    LCD_RST_HIGH();
    delay_ms(10);
    LCD_RST_LOW();
    delay_ms(10);
    LCD_RST_HIGH();
    delay_ms(120);
    
    /* Software reset */
    lcd_write_cmd(ST7735_SWRESET);
    delay_ms(150);
    
    /* Sleep out */
    lcd_write_cmd(ST7735_SLPOUT);
    delay_ms(120);
    
    /* Frame rate control */
    lcd_write_cmd(ST7735_FRMCTR1);
    lcd_write_data(0x01);
    lcd_write_data(0x2C);
    lcd_write_data(0x2D);
    
    lcd_write_cmd(ST7735_FRMCTR2);
    lcd_write_data(0x01);
    lcd_write_data(0x2C);
    lcd_write_data(0x2D);
    
    lcd_write_cmd(ST7735_FRMCTR3);
    lcd_write_data(0x01);
    lcd_write_data(0x2C);
    lcd_write_data(0x2D);
    lcd_write_data(0x01);
    lcd_write_data(0x2C);
    lcd_write_data(0x2D);
    
    /* Display inversion control */
    lcd_write_cmd(ST7735_INVCTR);
    lcd_write_data(0x07);
    
    /* Power control */
    lcd_write_cmd(ST7735_PWCTR1);
    lcd_write_data(0xA2);
    lcd_write_data(0x02);
    lcd_write_data(0x84);
    
    lcd_write_cmd(ST7735_PWCTR2);
    lcd_write_data(0xC5);
    
    lcd_write_cmd(ST7735_PWCTR3);
    lcd_write_data(0x0A);
    lcd_write_data(0x00);
    
    lcd_write_cmd(ST7735_PWCTR4);
    lcd_write_data(0x8A);
    lcd_write_data(0x2A);
    
    lcd_write_cmd(ST7735_PWCTR5);
    lcd_write_data(0x8A);
    lcd_write_data(0xEE);
    
    /* VCOM control */
    lcd_write_cmd(ST7735_VMCTR1);
    lcd_write_data(0x0E);
    
    /* Display orientation */
    lcd_write_cmd(ST7735_MADCTL);
    lcd_write_data(0x08);  /* RGB order */
    
    /* Color mode - 16bit */
    lcd_write_cmd(ST7735_COLMOD);
    lcd_write_data(0x05);
    
    /* Gamma correction */
    lcd_write_cmd(ST7735_GMCTRP1);
    lcd_write_data(0x0F);
    lcd_write_data(0x1A);
    lcd_write_data(0x0F);
    lcd_write_data(0x18);
    lcd_write_data(0x2F);
    lcd_write_data(0x28);
    lcd_write_data(0x20);
    lcd_write_data(0x22);
    lcd_write_data(0x1F);
    lcd_write_data(0x1B);
    lcd_write_data(0x23);
    lcd_write_data(0x37);
    lcd_write_data(0x00);
    lcd_write_data(0x07);
    lcd_write_data(0x02);
    lcd_write_data(0x10);
    
    lcd_write_cmd(ST7735_GMCTRN1);
    lcd_write_data(0x0F);
    lcd_write_data(0x1B);
    lcd_write_data(0x0F);
    lcd_write_data(0x17);
    lcd_write_data(0x33);
    lcd_write_data(0x2C);
    lcd_write_data(0x29);
    lcd_write_data(0x2E);
    lcd_write_data(0x30);
    lcd_write_data(0x30);
    lcd_write_data(0x39);
    lcd_write_data(0x3F);
    lcd_write_data(0x00);
    lcd_write_data(0x07);
    lcd_write_data(0x03);
    lcd_write_data(0x10);
    
    /* Normal display on */
    lcd_write_cmd(ST7735_NORON);
    delay_ms(10);
    
    /* Display on */
    lcd_write_cmd(ST7735_DISPON);
    delay_ms(100);
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

/* Color definitions */
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_BLUE      0x001F
#define COLOR_YELLOW    0xFFE0
#define COLOR_MAGENTA   0xF81F
#define COLOR_CYAN      0x07FF
#define COLOR_WHITE     0xFFFF
#define COLOR_BLACK     0x0000

int main(void) {
    /* Power latch first */
    Early_PowerLatch();
    
    /* Configure all GPIOB pins as outputs */
    GPIOB_MODER = 0x00005555;  /* PB0-PB7 as outputs */
    
    /* Initial states */
    GPIOB_ODR = (1 << LED_RED_PIN) |    /* LED OFF */
                (1 << BL1_PIN) |         /* BL1 OFF */
                (1 << LCD_CS_PIN) |      /* CS HIGH */
                (1 << LCD_RST_PIN);      /* RST HIGH */
    
    /* Turn on low brightness backlight first */
    GPIOB_ODR |= (1 << BL2_PIN);  /* PB2 HIGH = Low brightness ON */
    
    /* Blink once to show start */
    blink_led(1);
    delay_ms(500);
    
    /* Initialize LCD */
    lcd_init();
    
    /* Turn on high brightness after init */
    GPIOB_ODR &= ~(1 << BL1_PIN);  /* PB1 LOW = High brightness ON */
    
    /* Blink twice to show LCD init done */
    blink_led(2);
    
    /* Fill screen with red to test */
    lcd_fill_color(COLOR_RED);
    
    /* Blink 3 times to show color fill done */
    blink_led(3);
    
    /* Color cycle */
    uint16_t colors[] = {
        COLOR_RED, COLOR_GREEN, COLOR_BLUE, 
        COLOR_YELLOW, COLOR_MAGENTA, COLOR_CYAN, 
        COLOR_WHITE, COLOR_BLACK
    };
    
    int color_index = 0;
    
    /* Main loop - cycle through colors */
    while(1) {
        /* Fill screen with current color */
        lcd_fill_color(colors[color_index]);
        
        /* Quick LED blink to show color change */
        GPIOB_ODR &= ~(1 << LED_RED_PIN);
        delay_ms(100);
        GPIOB_ODR |= (1 << LED_RED_PIN);
        
        /* Wait 2 seconds */
        delay_ms(2000);
        
        /* Next color */
        color_index++;
        if(color_index >= 8) {
            color_index = 0;
        }
    }
    
    return 0;
}