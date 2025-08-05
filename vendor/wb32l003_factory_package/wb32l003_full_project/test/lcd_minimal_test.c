/**
 * LCD最小测试程序
 * 目的：验证LCD基本通信是否正常
 * 芯片：WB32L003K8U6
 * LCD：0.96寸 ST7735S 80×160
 */

#include "wb32l003.h"

/* 寄存器地址定义 */
#define RCC_AHBENR      (*(volatile uint32_t *)0x40021014)
#define GPIOB_MODER     (*(volatile uint32_t *)0x48000400)
#define GPIOB_ODR       (*(volatile uint32_t *)0x48000414)
#define GPIOC_MODER     (*(volatile uint32_t *)0x48000800)
#define GPIOC_ODR       (*(volatile uint32_t *)0x48000814)

/* LCD引脚定义 - 基于原理图 */
#define LCD_SCL_PIN     0   /* PC0 - 时钟 */
#define LCD_SDA_PIN     1   /* PC1 - 数据 */
#define LCD_RST_PIN     3   /* PC3 - 复位 */
#define LCD_RS_PIN      4   /* PC4 - 数据/命令 */
#define LCD_CS_PIN      6   /* PC6 - 片选（注意：与电源保持共用） */

/* 背光和LED */
#define BL1_PIN         1   /* PB1 - 高亮背光 */
#define BL2_PIN         2   /* PB2 - 低亮背光 */
#define LED_PIN         0   /* PB0 - 红色LED */

/* ST7735S基本命令 */
#define ST7735_SWRESET  0x01
#define ST7735_SLPOUT   0x11
#define ST7735_DISPON   0x29
#define ST7735_CASET    0x2A
#define ST7735_RASET    0x2B
#define ST7735_RAMWR    0x2C
#define ST7735_MADCTL   0x36
#define ST7735_COLMOD   0x3A

/* 简单延时 */
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        for(volatile uint32_t j = 0; j < 2400; j++);
    }
}

/* LED闪烁用于调试 */
void led_blink(int times) {
    for(int i = 0; i < times; i++) {
        GPIOB_ODR &= ~(1 << LED_PIN);  /* LED亮 */
        delay_ms(200);
        GPIOB_ODR |= (1 << LED_PIN);   /* LED灭 */
        delay_ms(200);
    }
}

/* LCD控制宏 */
#define LCD_CS_LOW()    (GPIOC_ODR &= ~(1 << LCD_CS_PIN))
#define LCD_CS_HIGH()   (GPIOC_ODR |= (1 << LCD_CS_PIN))
#define LCD_DC_LOW()    (GPIOC_ODR &= ~(1 << LCD_RS_PIN))
#define LCD_DC_HIGH()   (GPIOC_ODR |= (1 << LCD_RS_PIN))
#define LCD_SCK_LOW()   (GPIOC_ODR &= ~(1 << LCD_SCL_PIN))
#define LCD_SCK_HIGH()  (GPIOC_ODR |= (1 << LCD_SCL_PIN))
#define LCD_SDA_LOW()   (GPIOC_ODR &= ~(1 << LCD_SDA_PIN))
#define LCD_SDA_HIGH()  (GPIOC_ODR |= (1 << LCD_SDA_PIN))
#define LCD_RST_LOW()   (GPIOC_ODR &= ~(1 << LCD_RST_PIN))
#define LCD_RST_HIGH()  (GPIOC_ODR |= (1 << LCD_RST_PIN))

/* SPI发送一个字节 */
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

/* 写命令 */
void lcd_write_cmd(uint8_t cmd) {
    LCD_DC_LOW();
    LCD_CS_LOW();
    spi_write_byte(cmd);
    LCD_CS_HIGH();
}

/* 写数据 */
void lcd_write_data(uint8_t data) {
    LCD_DC_HIGH();
    LCD_CS_LOW();
    spi_write_byte(data);
    LCD_CS_HIGH();
}

/* LCD最简初始化 */
void lcd_init_minimal(void) {
    /* 硬件复位 */
    LCD_RST_HIGH();
    delay_ms(10);
    LCD_RST_LOW();
    delay_ms(10);
    LCD_RST_HIGH();
    delay_ms(120);
    
    /* 软件复位 */
    lcd_write_cmd(ST7735_SWRESET);
    delay_ms(150);
    
    /* 退出睡眠 */
    lcd_write_cmd(ST7735_SLPOUT);
    delay_ms(120);
    
    /* 显示方向 */
    lcd_write_cmd(ST7735_MADCTL);
    lcd_write_data(0xC0);  /* 竖屏显示 */
    
    /* 颜色格式 */
    lcd_write_cmd(ST7735_COLMOD);
    lcd_write_data(0x05);  /* 16bit RGB565 */
    
    /* 开启显示 */
    lcd_write_cmd(ST7735_DISPON);
    delay_ms(10);
}

/* 填充纯色测试 */
void lcd_fill_color(uint16_t color) {
    /* 设置显示区域 0,0 到 79,159 */
    lcd_write_cmd(ST7735_CASET);
    lcd_write_data(0x00);
    lcd_write_data(0x00);
    lcd_write_data(0x00);
    lcd_write_data(79);
    
    lcd_write_cmd(ST7735_RASET);
    lcd_write_data(0x00);
    lcd_write_data(0x00);
    lcd_write_data(0x00);
    lcd_write_data(159);
    
    /* 写入数据 */
    lcd_write_cmd(ST7735_RAMWR);
    for(int i = 0; i < 80 * 160; i++) {
        lcd_write_data(color >> 8);
        lcd_write_data(color & 0xFF);
    }
}

int main(void) {
    /* 启用GPIO时钟 */
    RCC_AHBENR |= (1 << 18) | (1 << 19);  /* GPIOB, GPIOC */
    
    /* 配置GPIOB - LED和背光 */
    GPIOB_MODER &= ~((3 << 0) | (3 << 2) | (3 << 4));
    GPIOB_MODER |= (1 << 0) | (1 << 2) | (1 << 4);
    
    /* 配置GPIOC - LCD控制 */
    GPIOC_MODER &= ~((3 << 0) | (3 << 2) | (3 << 6) | (3 << 8) | (3 << 12));
    GPIOC_MODER |= (1 << 0) | (1 << 2) | (1 << 6) | (1 << 8) | (1 << 12);
    
    /* 初始状态 */
    GPIOB_ODR = (1 << LED_PIN) | (1 << BL1_PIN);  /* LED灭，背光关 */
    GPIOC_ODR = (1 << LCD_CS_PIN) | (1 << LCD_RST_PIN);  /* CS高，RST高 */
    
    /* 开启低亮背光 */
    GPIOB_ODR |= (1 << BL2_PIN);
    
    /* LED闪烁1次表示开始 */
    led_blink(1);
    
    /* LCD初始化 */
    lcd_init_minimal();
    
    /* LED闪烁2次表示初始化完成 */
    led_blink(2);
    
    /* 填充红色测试 */
    lcd_fill_color(0xF800);  /* 红色 */
    
    /* LED闪烁3次表示测试完成 */
    led_blink(3);
    
    /* 开启高亮背光 */
    GPIOB_ODR &= ~(1 << BL1_PIN);
    
    /* 循环闪烁LED表示程序正常运行 */
    while(1) {
        led_blink(1);
        delay_ms(1000);
    }
    
    return 0;
}