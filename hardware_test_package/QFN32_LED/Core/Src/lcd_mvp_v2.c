/**
 * @file lcd_mvp_v2.c
 * @brief Minimal LCD driver for ST7735S - MVP version 2
 * @version 2.0
 * Updated based on backlight circuit analysis
 */

#include "lcd_mvp.h"

/* Pin control macros */
#define LCD_SCLK_LOW()  HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_RESET)
#define LCD_SCLK_HIGH() HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_SET)
#define LCD_MOSI_LOW()  HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_RESET)
#define LCD_MOSI_HIGH() HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_SET)
#define LCD_CS_LOW()    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()   HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_DC_LOW()    HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH()   HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET)
#define LCD_RST_LOW()   HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)
#define LCD_RST_HIGH()  HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)

/* 背光控制 - 基于 S8550 PNP 晶体管
 * 根据原理图，BLK 对应 J1 的第8脚
 * 我们需要确认具体映射到哪个 GPIO
 * 暂时假设 PB1 是 BLK 控制引脚
 */
#define LCD_BLK_PIN     CLD_BL1_PIN
#define LCD_BLK_PORT    CLD_BL1_PORT
#define LCD_BLK_ON()    HAL_GPIO_WritePin(LCD_BLK_PORT, LCD_BLK_PIN, GPIO_PIN_RESET)
#define LCD_BLK_OFF()   HAL_GPIO_WritePin(LCD_BLK_PORT, LCD_BLK_PIN, GPIO_PIN_SET)

/**
 * @brief Initialize LCD GPIO pins
 */
void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Enable GPIO clocks */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /* Configure LCD control pins */
    GPIO_InitStruct.Pin = LCD_RST_PIN | LCD_RS_PIN | LCD_SCL_PIN | 
                         LCD_CS_PIN | LCD_SDA_PIN | LCD_BLK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* Set initial states */
    LCD_CS_HIGH();
    LCD_SCLK_HIGH();
    LCD_MOSI_HIGH();
    LCD_RST_HIGH();
    LCD_DC_HIGH();
    LCD_BLK_OFF();  /* 初始关闭背光 */
}

/**
 * @brief Write 8-bit data via software SPI
 */
void LCD_WriteBus(uint8_t dat)
{
    uint8_t i;
    LCD_CS_LOW();
    
    for(i = 0; i < 8; i++)
    {
        LCD_SCLK_LOW();
        if(dat & 0x80) {
            LCD_MOSI_HIGH();
        } else {
            LCD_MOSI_LOW();
        }
        LCD_SCLK_HIGH();
        dat <<= 1;
    }
    
    LCD_CS_HIGH();
}

/**
 * @brief Write 8-bit data
 */
void LCD_WriteData8(uint8_t dat)
{
    LCD_WriteBus(dat);
}

/**
 * @brief Write 16-bit data
 */
void LCD_WriteData16(uint16_t dat)
{
    LCD_WriteBus(dat >> 8);
    LCD_WriteBus(dat);
}

/**
 * @brief Write command
 */
void LCD_WriteCommand(uint8_t cmd)
{
    LCD_DC_LOW();  /* Command mode */
    LCD_WriteBus(cmd);
    LCD_DC_HIGH(); /* Data mode */
}

/**
 * @brief Set display address window
 * Note: 根据应用场景，可能需要调整偏移
 */
void LCD_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    /* 尝试两种模式：
     * 1. 带偏移（京东方 demo）
     * 2. 不带偏移（标准模式）
     */
    #ifdef USE_X_OFFSET
        LCD_WriteCommand(0x2A);  /* Column address set */
        LCD_WriteData8(0x00);
        LCD_WriteData8(x1 + 24);
        LCD_WriteData8(0x00);
        LCD_WriteData8(x2 + 24);
    #else
        LCD_WriteCommand(0x2A);  /* Column address set */
        LCD_WriteData8(0x00);
        LCD_WriteData8(x1);
        LCD_WriteData8(0x00);
        LCD_WriteData8(x2);
    #endif
    
    LCD_WriteCommand(0x2B);  /* Row address set */
    LCD_WriteData8(0x00);
    LCD_WriteData8(y1);
    LCD_WriteData8(0x00);
    LCD_WriteData8(y2);
    
    LCD_WriteCommand(0x2C);  /* Memory write */
}

/**
 * @brief Initialize ST7735S display
 */
void LCD_Init(void)
{
    /* Initialize GPIO */
    LCD_GPIO_Init();
    
    /* Hardware reset */
    LCD_RST_LOW();
    HAL_Delay(100);
    LCD_RST_HIGH();
    HAL_Delay(100);
    
    /* Sleep exit */
    LCD_WriteCommand(0x11);
    HAL_Delay(120);
    
    /* Frame rate control - normal mode */
    LCD_WriteCommand(0xB1);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x3C);
    
    /* Frame rate control - idle mode */
    LCD_WriteCommand(0xB2);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x3C);
    
    /* Frame rate control - partial mode */
    LCD_WriteCommand(0xB3);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x3C);
    
    /* Display inversion control */
    LCD_WriteCommand(0xB4);
    LCD_WriteData8(0x03);
    
    /* Power control 1 */
    LCD_WriteCommand(0xC0);
    LCD_WriteData8(0x0E);
    LCD_WriteData8(0x0E);
    LCD_WriteData8(0x04);
    
    /* Power control 2 */
    LCD_WriteCommand(0xC1);
    LCD_WriteData8(0xC5);
    
    /* Power control 3 */
    LCD_WriteCommand(0xC2);
    LCD_WriteData8(0x0D);
    LCD_WriteData8(0x00);
    
    /* Power control 4 */
    LCD_WriteCommand(0xC3);
    LCD_WriteData8(0x8D);
    LCD_WriteData8(0x2A);
    
    /* Power control 5 */
    LCD_WriteCommand(0xC4);
    LCD_WriteData8(0x8D);
    LCD_WriteData8(0xEE);
    
    /* VCOM control */
    LCD_WriteCommand(0xC5);
    LCD_WriteData8(0x06);  /* 京东方 demo value */
    
    /* Memory data access control - portrait mode */
    LCD_WriteCommand(0x36);
    LCD_WriteData8(0x08);  /* MX=0, MY=0, MV=0, ML=0, RGB=1 */
    
    /* Interface pixel format - 16bit/pixel */
    LCD_WriteCommand(0x3A);
    LCD_WriteData8(0x55);
    
    /* Gamma correction + */
    LCD_WriteCommand(0xE0);
    LCD_WriteData8(0x0B);
    LCD_WriteData8(0x17);
    LCD_WriteData8(0x0A);
    LCD_WriteData8(0x0D);
    LCD_WriteData8(0x1A);
    LCD_WriteData8(0x19);
    LCD_WriteData8(0x16);
    LCD_WriteData8(0x1D);
    LCD_WriteData8(0x21);
    LCD_WriteData8(0x26);
    LCD_WriteData8(0x37);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x09);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x10);
    
    /* Gamma correction - */
    LCD_WriteCommand(0xE1);
    LCD_WriteData8(0x0C);
    LCD_WriteData8(0x19);
    LCD_WriteData8(0x09);
    LCD_WriteData8(0x0D);
    LCD_WriteData8(0x1B);
    LCD_WriteData8(0x19);
    LCD_WriteData8(0x15);
    LCD_WriteData8(0x1D);
    LCD_WriteData8(0x21);
    LCD_WriteData8(0x26);
    LCD_WriteData8(0x39);
    LCD_WriteData8(0x3E);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x09);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x10);
    
    HAL_Delay(120);
    
    /* Display on */
    LCD_WriteCommand(0x29);
    
    /* 等待显示稳定后再开背光 */
    HAL_Delay(50);
    LCD_BLK_ON();
}

/**
 * @brief Fill rectangle with color
 */
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    LCD_SetAddress(xsta, ysta, xend - 1, yend - 1);
    
    for(i = ysta; i < yend; i++)
    {
        for(j = xsta; j < xend; j++)
        {
            LCD_WriteData16(color);
        }
    }
}

/**
 * @brief Clear screen with color
 */
void LCD_Clear(uint16_t color)
{
    LCD_Fill(0, 0, LCD_W, LCD_H, color);
}