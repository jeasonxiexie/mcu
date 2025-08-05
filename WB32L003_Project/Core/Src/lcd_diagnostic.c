/**
 * @file lcd_diagnostic.c
 * @brief LCD diagnostic version - with LED indicators
 */

#include "wb32l003_hal.h"
#include "lcd_mvp.h"

/* LED indicator pins */
#define LED_GREEN_PIN   GPIO_PIN_15
#define LED_GREEN_PORT  GPIOB

/* External LCD functions */
extern void LCD_Init(void);
extern void LCD_Clear(uint16_t color);
extern void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);

/* Simple delay for visible LED blinks */
void LED_Delay(uint32_t count)
{
    volatile uint32_t i;
    for(i = 0; i < count * 1000; i++);
}

/* Blink LED n times */
void LED_Blink(uint8_t times)
{
    uint8_t i;
    for(i = 0; i < times; i++)
    {
        HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
        LED_Delay(200);
        HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
        LED_Delay(200);
    }
}

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Initialize HAL */
    HAL_Init();
    
    /* Configure power latch - PC6 LOW */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
    
    /* Configure LED for diagnostics */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = LED_GREEN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct);
    
    /* Diagnostic: 1 blink = power on */
    LED_Blink(1);
    HAL_Delay(500);
    
    /* Initialize LCD */
    LCD_Init();
    
    /* Diagnostic: 2 blinks = LCD initialized */
    LED_Blink(2);
    HAL_Delay(500);
    
    /* Main loop with CPU load to generate heat */
    while (1)
    {
        /* Diagnostic: 3 blinks = entering main loop */
        LED_Blink(3);
        
        /* White screen */
        LCD_Clear(0xFFFF);
        
        /* CPU intensive operation to generate heat */
        volatile uint32_t dummy = 0;
        for(uint32_t i = 0; i < 1000000; i++)
        {
            dummy += i * i;
        }
        
        HAL_Delay(1000);
        
        /* Red screen */
        LCD_Clear(0xF800);
        
        /* More CPU load */
        for(uint32_t i = 0; i < 1000000; i++)
        {
            dummy += i * i;
        }
        
        HAL_Delay(1000);
        
        /* Green screen */
        LCD_Clear(0x07E0);
        
        /* More CPU load */
        for(uint32_t i = 0; i < 1000000; i++)
        {
            dummy += i * i;
        }
        
        HAL_Delay(1000);
        
        /* Blue screen */
        LCD_Clear(0x001F);
        
        /* More CPU load */
        for(uint32_t i = 0; i < 1000000; i++)
        {
            dummy += i * i;
        }
        
        HAL_Delay(1000);
    }
}