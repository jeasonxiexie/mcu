/**
 * @file lcd_test_corrected.c
 * @brief LCD test with corrected GPIOB pin mappings
 */

#include "wb32l003_hal.h"
#include "lcd_init_correct.h"

// Early power latch - external function from early_power.c
extern void Early_PowerLatch(void);

// System clock configuration
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    // Configure HSI oscillator
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    
    // Configure system clocks
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}

// Simple test pattern
void LCD_TestPattern(void)
{
    // Fill screen with red
    LCD_Fill(0, 0, LCD_W, LCD_H, RED);
    HAL_Delay(1000);
    
    // Fill screen with green
    LCD_Fill(0, 0, LCD_W, LCD_H, GREEN);
    HAL_Delay(1000);
    
    // Fill screen with blue
    LCD_Fill(0, 0, LCD_W, LCD_H, BLUE);
    HAL_Delay(1000);
    
    // Draw white border
    LCD_Fill(0, 0, LCD_W, 2, WHITE);           // Top
    LCD_Fill(0, LCD_H-2, LCD_W, LCD_H, WHITE); // Bottom
    LCD_Fill(0, 0, 2, LCD_H, WHITE);           // Left
    LCD_Fill(LCD_W-2, 0, LCD_W, LCD_H, WHITE); // Right
    
    // Draw colorful pattern in center
    LCD_Fill(10, 10, 30, 30, RED);
    LCD_Fill(30, 30, 50, 50, GREEN);
    LCD_Fill(50, 50, 70, 70, BLUE);
    LCD_Fill(35, 75, 45, 85, YELLOW);
    LCD_Fill(20, 100, 60, 120, CYAN);
    LCD_Fill(10, 130, 70, 150, MAGENTA);
}

// Main function
int main(void)
{
    // Initialize HAL library
    HAL_Init();
    
    // Configure system clock
    SystemClock_Config();
    
    // Early power latch - keep system powered
    Early_PowerLatch();
    
    // Initialize LCD with corrected pin mappings
    LCD_Init();
    
    // Main loop
    while (1)
    {
        // Run test pattern
        LCD_TestPattern();
        
        // Flash the LEDs to show activity
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0); // Red LED
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1); // Green LED
        HAL_Delay(500);
    }
}

// Error handler
void Error_Handler(void)
{
    // User can add implementation to report the error
    while(1)
    {
        // Flash red LED on error
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
        HAL_Delay(100);
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    // User can add implementation to report the file name and line number
    // printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif