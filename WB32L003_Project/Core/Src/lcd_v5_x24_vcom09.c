/**
 * @file lcd_test_base.c
 * @brief Base LCD test framework for configuration variants
 */

/* Configuration parameters - will be modified for each variant */
#define X_OFFSET        24      /* X-axis offset: 0, 24, 26, 32 */
#define Y_OFFSET        0       /* Y-axis offset: 0, 24 */
#define VCOM_VALUE      0x09    /* VCOM: 0x06, 0x09, 0x1D */
#define MADCTL_VALUE    0x08    /* Display orientation */
#define USE_HORIZONTAL  0       /* 0=Portrait, 1=Landscape */

/* Register definitions */
#define RCC_AHBENR  (*(volatile unsigned int*)0x40021014)
#define GPIOB_MODER (*(volatile unsigned int*)0x48000400)
#define GPIOB_ODR   (*(volatile unsigned int*)0x48000414)
#define GPIOC_MODER (*(volatile unsigned int*)0x48000800)
#define GPIOC_ODR   (*(volatile unsigned int*)0x48000814)

/* LCD pins on GPIOB */
#define LCD_RST_PIN  7
#define LCD_RS_PIN   6
#define LCD_SCL_PIN  5
#define LCD_CS_PIN   4
#define LCD_SDA_PIN  3
#define LCD_BL1_PIN  1
#define LCD_BL2_PIN  2

/* Delay function */
void delay_ms(unsigned int ms)
{
    for(unsigned int i = 0; i < ms; i++) {
        for(volatile unsigned int j = 0; j < 4800; j++);
    }
}

/* SPI write byte */
void spi_write_byte(unsigned char data)
{
    GPIOB_ODR &= ~(1 << LCD_CS_PIN);
    
    for(int i = 7; i >= 0; i--)
    {
        GPIOB_ODR &= ~(1 << LCD_SCL_PIN);
        
        if(data & (1 << i))
            GPIOB_ODR |= (1 << LCD_SDA_PIN);
        else
            GPIOB_ODR &= ~(1 << LCD_SDA_PIN);
            
        GPIOB_ODR |= (1 << LCD_SCL_PIN);
    }
    
    GPIOB_ODR |= (1 << LCD_CS_PIN);
}

/* LCD commands */
void lcd_write_cmd(unsigned char cmd)
{
    GPIOB_ODR &= ~(1 << LCD_RS_PIN);
    spi_write_byte(cmd);
    GPIOB_ODR |= (1 << LCD_RS_PIN);
}

void lcd_write_data(unsigned char data)
{
    spi_write_byte(data);
}

void lcd_write_data16(unsigned short data)
{
    lcd_write_data(data >> 8);
    lcd_write_data(data & 0xFF);
}

/* Set address window */
void lcd_set_window(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    /* Apply offsets based on configuration */
    #if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
        /* Portrait mode */
        lcd_write_cmd(0x2A);
        lcd_write_data(0x00);
        lcd_write_data(x1 + X_OFFSET);
        lcd_write_data(0x00);
        lcd_write_data(x2 + X_OFFSET);
        
        lcd_write_cmd(0x2B);
        lcd_write_data(0x00);
        lcd_write_data(y1 + Y_OFFSET);
        lcd_write_data(0x00);
        lcd_write_data(y2 + Y_OFFSET);
    #else
        /* Landscape mode */
        lcd_write_cmd(0x2A);
        lcd_write_data(0x00);
        lcd_write_data(x1 + Y_OFFSET);
        lcd_write_data(0x00);
        lcd_write_data(x2 + Y_OFFSET);
        
        lcd_write_cmd(0x2B);
        lcd_write_data(0x00);
        lcd_write_data(y1 + X_OFFSET);
        lcd_write_data(0x00);
        lcd_write_data(y2 + X_OFFSET);
    #endif
    
    lcd_write_cmd(0x2C);
}

/* Initialize LCD */
void lcd_init(void)
{
    /* Hardware reset */
    GPIOB_ODR &= ~(1 << LCD_RST_PIN);
    delay_ms(100);
    GPIOB_ODR |= (1 << LCD_RST_PIN);
    delay_ms(100);
    
    /* Sleep out */
    lcd_write_cmd(0x11);
    delay_ms(120);
    
    /* Frame rate control - normal mode */
    lcd_write_cmd(0xB1);
    lcd_write_data(0x05);
    lcd_write_data(0x3C);
    lcd_write_data(0x3C);
    
    /* Frame rate control - idle mode */
    lcd_write_cmd(0xB2);
    lcd_write_data(0x05);
    lcd_write_data(0x3C);
    lcd_write_data(0x3C);
    
    /* Frame rate control - partial mode */
    lcd_write_cmd(0xB3);
    lcd_write_data(0x05);
    lcd_write_data(0x3C);
    lcd_write_data(0x3C);
    lcd_write_data(0x05);
    lcd_write_data(0x3C);
    lcd_write_data(0x3C);
    
    /* Display inversion control */
    lcd_write_cmd(0xB4);
    lcd_write_data(0x03);
    
    /* Power control 1 */
    lcd_write_cmd(0xC0);
    lcd_write_data(0x0E);
    lcd_write_data(0x0E);
    lcd_write_data(0x04);
    
    /* Power control 2 */
    lcd_write_cmd(0xC1);
    lcd_write_data(0xC5);
    
    /* Power control 3 */
    lcd_write_cmd(0xC2);
    lcd_write_data(0x0D);
    lcd_write_data(0x00);
    
    /* Power control 4 */
    lcd_write_cmd(0xC3);
    lcd_write_data(0x8D);
    lcd_write_data(0x2A);
    
    /* Power control 5 */
    lcd_write_cmd(0xC4);
    lcd_write_data(0x8D);
    lcd_write_data(0xEE);
    
    /* VCOM control */
    lcd_write_cmd(0xC5);
    lcd_write_data(VCOM_VALUE);
    
    /* Memory data access control */
    lcd_write_cmd(0x36);
    lcd_write_data(MADCTL_VALUE);
    
    /* Interface pixel format */
    lcd_write_cmd(0x3A);
    lcd_write_data(0x55);
    
    /* Gamma + correction */
    lcd_write_cmd(0xE0);
    lcd_write_data(0x0B);
    lcd_write_data(0x17);
    lcd_write_data(0x0A);
    lcd_write_data(0x0D);
    lcd_write_data(0x1A);
    lcd_write_data(0x19);
    lcd_write_data(0x16);
    lcd_write_data(0x1D);
    lcd_write_data(0x21);
    lcd_write_data(0x26);
    lcd_write_data(0x37);
    lcd_write_data(0x3C);
    lcd_write_data(0x00);
    lcd_write_data(0x09);
    lcd_write_data(0x05);
    lcd_write_data(0x10);
    
    /* Gamma - correction */
    lcd_write_cmd(0xE1);
    lcd_write_data(0x0C);
    lcd_write_data(0x19);
    lcd_write_data(0x09);
    lcd_write_data(0x0D);
    lcd_write_data(0x1B);
    lcd_write_data(0x19);
    lcd_write_data(0x15);
    lcd_write_data(0x1D);
    lcd_write_data(0x21);
    lcd_write_data(0x26);
    lcd_write_data(0x39);
    lcd_write_data(0x3E);
    lcd_write_data(0x00);
    lcd_write_data(0x09);
    lcd_write_data(0x05);
    lcd_write_data(0x10);
    
    delay_ms(120);
    
    /* Display on */
    lcd_write_cmd(0x29);
}

/* Early power latch */
void Early_PowerLatch(void)
{
    RCC_AHBENR |= (1 << 19);
    GPIOC_MODER &= ~(3 << 12);
    GPIOC_MODER |= (1 << 12);
    GPIOC_ODR &= ~(1 << 6);
}

/* Main function */
int main(void)
{
    /* Enable GPIOB clock */
    RCC_AHBENR |= (1 << 18);
    
    /* Configure LCD pins as outputs */
    unsigned int pins[] = {LCD_RST_PIN, LCD_RS_PIN, LCD_SCL_PIN, 
                          LCD_CS_PIN, LCD_SDA_PIN, LCD_BL1_PIN, LCD_BL2_PIN};
    
    for(int i = 0; i < 7; i++)
    {
        GPIOB_MODER &= ~(3 << (pins[i] * 2));
        GPIOB_MODER |= (1 << (pins[i] * 2));
    }
    
    /* Initial pin states */
    GPIOB_ODR |= (1 << LCD_CS_PIN);
    GPIOB_ODR |= (1 << LCD_SCL_PIN);
    GPIOB_ODR |= (1 << LCD_RST_PIN);
    GPIOB_ODR |= (1 << LCD_RS_PIN);
    
    /* Turn on backlight (PNP transistor: LOW = ON) */
    GPIOB_ODR &= ~(1 << LCD_BL1_PIN);
    delay_ms(50);
    
    /* Initialize LCD */
    lcd_init();
    
    /* Clear screen with test pattern */
    #if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
        lcd_set_window(0, 0, 79, 159);
    #else
        lcd_set_window(0, 0, 159, 79);
    #endif
    
    /* Fill with gradient pattern */
    for(int y = 0; y < 160; y++)
    {
        for(int x = 0; x < 80; x++)
        {
            unsigned short color = ((y >> 3) << 11) | ((x >> 2) << 5) | (y >> 3);
            lcd_write_data16(color);
        }
    }
    
    /* Keep running */
    while(1)
    {
        delay_ms(1000);
    }
}

/* Stubs */
void SystemInit(void) {}
void __libc_init_array(void) {}