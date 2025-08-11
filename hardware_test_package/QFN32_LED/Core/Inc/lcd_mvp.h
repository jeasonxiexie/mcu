/**
 * @file lcd_mvp.h
 * @brief Minimal LCD driver for ST7735S - MVP version
 * @version 1.0
 */

#ifndef __LCD_MVP_H
#define __LCD_MVP_H

#include "wb32l003.h"
#include "pin_config.h"

/* Display dimensions */
#define LCD_W 80
#define LCD_H 160

/* Color definitions (RGB565) */
#define WHITE   0xFFFF
#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define YELLOW  0xFFE0
#define CYAN    0x07FF
#define MAGENTA 0xF81F

/* Function prototypes */
void LCD_GPIO_Init(void);
void LCD_WriteBus(uint8_t dat);
void LCD_WriteData8(uint8_t dat);
void LCD_WriteData16(uint16_t dat);
void LCD_WriteCommand(uint8_t cmd);
void LCD_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Init(void);
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void LCD_Clear(uint16_t color);

#endif /* __LCD_MVP_H */