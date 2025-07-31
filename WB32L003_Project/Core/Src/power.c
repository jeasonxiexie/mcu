#include "power.h"
#include "ui.h"
#include "st7735.h"
#include "hardware_control.h"
#include "pin_config.h"

/* Forward declaration */
void SystemClock_Config(void);

extern ADC_HandleTypeDef hadc;

static power_mode_t power_mode = POWER_MODE_NORMAL;
static float battery_voltage = 3.7f;
static battery_level_t battery_level = BATTERY_LEVEL_3;
static uint32_t last_adc_time = 0;
static uint32_t led_flash_time = 0;
static uint8_t led_state = 0;

void Power_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    
    HAL_ADCEx_Calibration_Start(&hadc);
}

float Power_GetBatteryVoltage(void)
{
    uint32_t adc_value = 0;
    
    HAL_ADC_Start(&hadc);
    if (HAL_ADC_PollForConversion(&hadc, 100) == HAL_OK)
    {
        adc_value = HAL_ADC_GetValue(&hadc);
    }
    HAL_ADC_Stop(&hadc);
    
    battery_voltage = (float)adc_value * ADC_VREF / ADC_RESOLUTION;
    
    // Apply voltage divider ratio from pin_config.h (R52:R53 = 10K:33K = 1:4.3)
    battery_voltage = battery_voltage * BATTERY_VOLTAGE_DIVIDER;
    
    return battery_voltage;
}

battery_level_t Power_GetBatteryLevel(void)
{
    float voltage = battery_voltage;
    
    if (voltage >= 3.9f)
    {
        battery_level = BATTERY_LEVEL_4;
    }
    else if (voltage >= 3.7f)
    {
        battery_level = BATTERY_LEVEL_3;
    }
    else if (voltage >= 3.5f)
    {
        battery_level = BATTERY_LEVEL_2;
    }
    else if (voltage >= 3.2f)
    {
        battery_level = BATTERY_LEVEL_1;
    }
    else
    {
        battery_level = BATTERY_LEVEL_0;
    }
    
    return battery_level;
}

void Power_Process(void)
{
    uint32_t current_time = HAL_GetTick();
    
    if (current_time - last_adc_time >= 1000)
    {
        last_adc_time = current_time;
        
        float voltage = Power_GetBatteryVoltage();
        battery_level_t level = Power_GetBatteryLevel();
        
        UI_UpdateBatteryIcon(level);
        
        if (voltage < BATTERY_CRITICAL_VOLTAGE)
        {
            Power_Off();
        }
        else if (voltage < BATTERY_LOW_VOLTAGE)
        {
            // Flash red LED
            if (current_time - led_flash_time >= 500)
            {
                led_flash_time = current_time;
                led_state = !led_state;
                HW_SetRedLED(led_state);
                HW_SetGreenLED(false);
            }
        }
        else
        {
            // Normal state - green LED on
            HW_SetGreenLED(true);
            HW_SetRedLED(false);
        }
    }
}

power_mode_t Power_GetMode(void)
{
    return power_mode;
}

void Power_SetMode(power_mode_t mode)
{
    power_mode = mode;
}

void Power_Off(void)
{
    // Use hardware power-off sequence
    HW_PowerOffSequence();
}

void Power_EnterStandby(void)
{
    HAL_SuspendTick();
    
    __HAL_RCC_PWR_CLK_ENABLE();
    
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
    
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    
    SystemClock_Config();
    HAL_ResumeTick();
    
    Power_ExitStandby();
}

void Power_ExitStandby(void)
{
    power_mode = POWER_MODE_NORMAL;
    
    ST7735_WriteCommand(ST7735_SLPOUT);
    HAL_Delay(120);
    ST7735_WriteCommand(ST7735_DISPON);
    
    ST7735_SetBacklight(2);
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}