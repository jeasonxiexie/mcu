/**
 * @file lcd_variant_test.c
 * @brief Test program for LCD variants
 */

#include "wb32l003_hal.h"
#include "lcd_mvp.h"

/* Color definitions if not already defined */
#ifndef COLOR_WHITE
#define COLOR_WHITE     0xFFFF
#define COLOR_BLACK     0x0000
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_BLUE      0x001F
#define COLOR_YELLOW    0xFFE0
#define COLOR_CYAN      0x07FF
#define COLOR_MAGENTA   0xF81F
#endif

/* External LCD functions */
extern void LCD_Init(void);
extern void LCD_Clear(uint16_t color);
extern void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Initialize HAL */
    HAL_Init();
    
    /* Configure early power to latch on - PC6 LOW */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
    
    /* Initialize LCD */
    LCD_Init();
    
    /* Test pattern */
    while (1)
    {
        /* White screen */
        LCD_Clear(COLOR_WHITE);
        HAL_Delay(1000);
        
        /* Red screen */
        LCD_Clear(COLOR_RED);
        HAL_Delay(1000);
        
        /* Green screen */
        LCD_Clear(COLOR_GREEN);
        HAL_Delay(1000);
        
        /* Blue screen */
        LCD_Clear(COLOR_BLUE);
        HAL_Delay(1000);
        
        /* Four quadrants test */
        LCD_Fill(0, 0, LCD_W/2, LCD_H/2, COLOR_YELLOW);      /* Top-left: Yellow */
        LCD_Fill(LCD_W/2, 0, LCD_W, LCD_H/2, COLOR_CYAN);    /* Top-right: Cyan */
        LCD_Fill(0, LCD_H/2, LCD_W/2, LCD_H, COLOR_MAGENTA); /* Bottom-left: Magenta */
        LCD_Fill(LCD_W/2, LCD_H/2, LCD_W, LCD_H, COLOR_WHITE); /* Bottom-right: White */
        HAL_Delay(2000);
    }
}