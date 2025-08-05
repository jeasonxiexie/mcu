/**
 * @file screen_power_test.c  
 * @brief Screen initialization with power management
 */

/* Register definitions */
#define RCC_AHBENR  (*(volatile unsigned int*)0x40021014)
#define GPIOB_MODER (*(volatile unsigned int*)0x48000400)
#define GPIOB_ODR   (*(volatile unsigned int*)0x48000414)
#define GPIOC_MODER (*(volatile unsigned int*)0x48000800)
#define GPIOC_ODR   (*(volatile unsigned int*)0x48000814)

/* LCD pins on GPIOB */
#define LCD_RST_PIN  7  /* PB7 */
#define LCD_RS_PIN   6  /* PB6 */
#define LCD_SCL_PIN  5  /* PB5 */
#define LCD_CS_PIN   4  /* PB4 */
#define LCD_SDA_PIN  3  /* PB3 */
#define LCD_BL1_PIN  1  /* PB1 */

/* Simple delay */
void delay_ms(unsigned int ms)
{
    for(unsigned int i = 0; i < ms; i++) {
        for(volatile unsigned int j = 0; j < 4800; j++);
    }
}

/* SPI write byte */
void spi_write_byte(unsigned char data)
{
    /* CS LOW */
    GPIOB_ODR &= ~(1 << LCD_CS_PIN);
    
    for(int i = 7; i >= 0; i--)
    {
        /* SCL LOW */
        GPIOB_ODR &= ~(1 << LCD_SCL_PIN);
        
        /* Set MOSI */
        if(data & (1 << i))
            GPIOB_ODR |= (1 << LCD_SDA_PIN);
        else
            GPIOB_ODR &= ~(1 << LCD_SDA_PIN);
            
        /* SCL HIGH */
        GPIOB_ODR |= (1 << LCD_SCL_PIN);
    }
    
    /* CS HIGH */
    GPIOB_ODR |= (1 << LCD_CS_PIN);
}

/* Write command */
void lcd_write_cmd(unsigned char cmd)
{
    /* RS LOW for command */
    GPIOB_ODR &= ~(1 << LCD_RS_PIN);
    spi_write_byte(cmd);
    /* RS HIGH for data */
    GPIOB_ODR |= (1 << LCD_RS_PIN);
}

/* Write data */
void lcd_write_data(unsigned char data)
{
    spi_write_byte(data);
}

/* Early power latch */
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
    /* Enable GPIOB clock */
    RCC_AHBENR |= (1 << 18);
    
    /* Configure all LCD pins as outputs */
    unsigned int pins[] = {LCD_RST_PIN, LCD_RS_PIN, LCD_SCL_PIN, 
                          LCD_CS_PIN, LCD_SDA_PIN, LCD_BL1_PIN};
    
    for(int i = 0; i < 6; i++)
    {
        GPIOB_MODER &= ~(3 << (pins[i] * 2));
        GPIOB_MODER |= (1 << (pins[i] * 2));
    }
    
    /* Initial states */
    GPIOB_ODR |= (1 << LCD_CS_PIN);   /* CS HIGH */
    GPIOB_ODR |= (1 << LCD_SCL_PIN);  /* SCL HIGH */
    GPIOB_ODR |= (1 << LCD_RST_PIN);  /* RST HIGH */
    GPIOB_ODR |= (1 << LCD_RS_PIN);   /* RS HIGH */
    
    /* Hardware reset */
    GPIOB_ODR &= ~(1 << LCD_RST_PIN);
    delay_ms(100);
    GPIOB_ODR |= (1 << LCD_RST_PIN);
    delay_ms(100);
    
    /* Turn on backlight (PNP transistor: LOW = ON) */
    GPIOB_ODR &= ~(1 << LCD_BL1_PIN);
    
    /* Basic ST7735S initialization */
    lcd_write_cmd(0x11);  /* Sleep out */
    delay_ms(120);
    
    /* Frame rate */
    lcd_write_cmd(0xB1);
    lcd_write_data(0x05);
    lcd_write_data(0x3C);
    lcd_write_data(0x3C);
    
    /* Power control */
    lcd_write_cmd(0xC0);
    lcd_write_data(0x0E);
    lcd_write_data(0x0E);
    lcd_write_data(0x04);
    
    /* VCOM control */
    lcd_write_cmd(0xC5);
    lcd_write_data(0x06);  /* BOE default */
    
    /* Memory access control */
    lcd_write_cmd(0x36);
    lcd_write_data(0x08);
    
    /* Interface pixel format */
    lcd_write_cmd(0x3A);
    lcd_write_data(0x55);  /* 16-bit color */
    
    /* Display on */
    lcd_write_cmd(0x29);
    
    /* Fill screen with color pattern */
    lcd_write_cmd(0x2A);  /* Column address */
    lcd_write_data(0x00);
    lcd_write_data(24);    /* X start with offset */
    lcd_write_data(0x00);
    lcd_write_data(24 + 79); /* X end */
    
    lcd_write_cmd(0x2B);  /* Row address */
    lcd_write_data(0x00);
    lcd_write_data(0);     /* Y start */
    lcd_write_data(0x00);
    lcd_write_data(159);   /* Y end */
    
    lcd_write_cmd(0x2C);  /* Memory write */
    
    /* Write test pattern */
    for(int i = 0; i < 80 * 160; i++)
    {
        lcd_write_data(0xF8);  /* Red */
        lcd_write_data(0x00);
    }
    
    /* Keep running */
    while(1)
    {
        delay_ms(1000);
    }
}

/* Empty stubs */
void SystemInit(void) {}
void __libc_init_array(void) {}