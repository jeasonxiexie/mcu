/**
 * @file mvp_test.c
 * @brief MVP test program - minimal LCD test
 * @version 1.0
 */

#include "wb32l003_hal.h"
#include "pin_config.h"
#include "lcd_mvp.h"

/* System Clock Configuration */
static void SystemClock_Config(void)
{
    /* Configure system clock to 48 MHz using HSI */
    /* Default configuration should work */
}

/* GPIO Initialization */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable all GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

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

    /* Configure LED pins (for debug indication) */
    GPIO_InitStruct.Pin = LED_GREEN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct);

    /* LED OFF initially (HIGH = OFF for common anode) */
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
}

int main(void)
{
    /* Early power latch is already called in startup code */
    
    /* Reset of all peripherals, Initializes the Flash interface and the Systick */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();

    /* Small delay to ensure power stability */
    HAL_Delay(100);

    /* Initialize LCD */
    LCD_Init();

    /* Turn on green LED to indicate running */
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);  /* LOW = ON */

    /* Initial clear screen */
    LCD_Clear(BLACK);
    HAL_Delay(500);

    /* Main loop - simple color test */
    while (1)
    {
        /* White screen */
        LCD_Clear(WHITE);
        HAL_Delay(1000);

        /* Red screen */
        LCD_Clear(RED);
        HAL_Delay(1000);

        /* Green screen */
        LCD_Clear(GREEN);
        HAL_Delay(1000);

        /* Blue screen */
        LCD_Clear(BLUE);
        HAL_Delay(1000);

        /* Test fill patterns */
        LCD_Fill(0, 0, 40, 80, YELLOW);      /* Top-left quadrant */
        LCD_Fill(40, 0, 80, 80, CYAN);       /* Top-right quadrant */
        LCD_Fill(0, 80, 40, 160, MAGENTA);   /* Bottom-left quadrant */
        LCD_Fill(40, 80, 80, 160, WHITE);    /* Bottom-right quadrant */
        HAL_Delay(2000);

        /* Black screen before restart */
        LCD_Clear(BLACK);
        HAL_Delay(500);
    }
}