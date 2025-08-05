/**
 * @file screen_test.c
 * @brief Display test program for TFT LCD - RGB color cycling
 * @version 1.0
 */

#include "wb32l003_hal.h"
#include "pin_config.h"
#include "hardware_control.h"
#include "st7735.h"

/* System Clock Configuration */
static void SystemClock_Config(void)
{
    /* Configure the system clock to 48 MHz */
    /* Implementation depends on your clock requirements */
}

/* GPIO Initialization */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable all GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /* Some WB32L003 variants have GPIOD */
    #ifdef __HAL_RCC_GPIOD_CLK_ENABLE
        #ifdef RCC_AHBENR_GPIODEN
            __HAL_RCC_GPIOD_CLK_ENABLE();
        #endif
    #endif

    /* Configure LCD pins (PB7-PB0) */
    GPIO_InitStruct.Pin = LCD_RST_PIN | LCD_RS_PIN | LCD_SCL_PIN | 
                         LCD_CS_PIN | LCD_SDA_PIN | CLD_BL2_PIN | CLD_BL1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Configure power control pins */
    GPIO_InitStruct.Pin = CON_POW_CPU_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CON_POW_CPU_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CON_POW_LCD_PIN;
    HAL_GPIO_Init(CON_POW_LCD_PORT, &GPIO_InitStruct);

    /* Keep power on */
    HAL_GPIO_WritePin(CON_POW_CPU_PORT, CON_POW_CPU_PIN, GPIO_PIN_RESET);  /* PC6 LOW = ON */
    HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, GPIO_PIN_RESET);  /* PA3 LOW = ON */

    /* Configure LED pins */
    GPIO_InitStruct.Pin = LED_RED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_RED_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED_GREEN_PIN;
    HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct);

    /* LEDs OFF (HIGH = OFF for common anode) */
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
}

int main(void)
{
    /* MCU Configuration */
    
    /* Early power latch is already called in startup code */
    
    /* Reset of all peripherals, Initializes the Flash interface and the Systick */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();

    /* Initialize display */
    ST7735_Init();

    /* Turn on backlight at high brightness */
    ST7735_SetBacklight(2);

    /* Turn on green LED to indicate running */
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);  /* LOW = ON */

    /* Main loop - RGB color cycling like demo */
    while (1)
    {
        /* Red screen */
        ST7735_RGB_UPDATE(0x1F, 0x00, 0x00);  /* Max red */
        HAL_Delay(1000);

        /* Green screen */
        ST7735_RGB_UPDATE(0x00, 0x3F, 0x00);  /* Max green */
        HAL_Delay(1000);

        /* Blue screen */
        ST7735_RGB_UPDATE(0x00, 0x00, 0x1F);  /* Max blue */
        HAL_Delay(1000);

        /* White screen */
        ST7735_RGB_UPDATE(0x1F, 0x3F, 0x1F);  /* White */
        HAL_Delay(1000);

        /* Black screen */
        ST7735_RGB_UPDATE(0x00, 0x00, 0x00);  /* Black */
        HAL_Delay(1000);

        /* Yellow screen */
        ST7735_RGB_UPDATE(0x1F, 0x3F, 0x00);  /* Yellow */
        HAL_Delay(1000);

        /* Cyan screen */
        ST7735_RGB_UPDATE(0x00, 0x3F, 0x1F);  /* Cyan */
        HAL_Delay(1000);

        /* Magenta screen */
        ST7735_RGB_UPDATE(0x1F, 0x00, 0x1F);  /* Magenta */
        HAL_Delay(1000);
    }
}