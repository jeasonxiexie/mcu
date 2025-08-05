/**
 * @file main.c
 * @brief Main application template for STM32 Clean Architecture
 * 
 * This is a template main.c file that demonstrates how to use the clean architecture
 * framework for STM32 projects. It can be customized for specific applications.
 */

#include "pin_config.h"
#include "../core/interfaces/display_interface.h"
#include "../core/interfaces/power_interface.h"
#include "../core/interfaces/input_interface.h"
#include "../core/interfaces/audio_interface.h"
#include "../core/usecases/power_management.h"
#include "../core/usecases/audio_management.h"
#include "../core/usecases/ui_management.h"
#include "../adapters/display/st7735_adapter.h"
#include "../adapters/power/power_adapter.h"
#include "../adapters/input/button_adapter.h"
#include "../adapters/audio/audio_adapter.h"
#include "../frameworks/drivers/system.h"

/* Private variables */
static bool system_initialized = false;

/* Private function declarations */
static void SystemClock_Config(void);
static void GPIO_Init(void);
static void SPI1_Init(void);
static void TIM2_Init(void);
static void ADC_Init(void);
static bool Initialize_Interfaces(void);
static void Main_Loop(void);
static void Handle_System_Events(void);

/**
 * @brief Main application entry point
 */
int main(void)
{
    /* HAL Library initialization */
    HAL_Init();
    
    /* Configure system clock */
    SystemClock_Config();
    
    /* Initialize GPIO */
    GPIO_Init();
    
    /* Initialize peripherals */
    SPI1_Init();
    TIM2_Init();
    ADC_Init();
    
    /* Initialize clean architecture interfaces */
    if (!Initialize_Interfaces()) {
        /* Failed to initialize - handle error */
        Error_Handler();
    }
    
    system_initialized = true;
    
    /* Show startup logo */
    ui_mgmt_show_startup_sequence();
    
    /* Main application loop */
    Main_Loop();
    
    /* Should never reach here */
    while (1) {
        /* Error state */
    }
}

/**
 * @brief Initialize all clean architecture interfaces
 */
static bool Initialize_Interfaces(void)
{
    /* Initialize display interface */
    if (!st7735_display_interface.init()) {
        return false;
    }
    
    /* Initialize power management */
    if (!power_mgmt_init(&power_adapter_interface, &st7735_display_interface)) {
        return false;
    }
    
    /* Initialize input interface */
    if (!button_adapter_interface.init()) {
        return false;
    }
    
    /* Initialize audio interface */
    if (!audio_adapter_interface.init()) {
        return false;
    }
    
    /* Initialize audio management */
    if (!audio_mgmt_init(&audio_adapter_interface)) {
        return false;
    }
    
    /* Initialize UI management */
    if (!ui_mgmt_init(&st7735_display_interface, &audio_adapter_interface)) {
        return false;
    }
    
    return true;
}

/**
 * @brief Main application loop
 */
static void Main_Loop(void)
{
    uint32_t last_ui_update = 0;
    uint32_t last_audio_update = 0;
    
    while (1) {
        uint32_t current_time = HAL_GetTick();
        
        /* Process power management (every cycle) */
        power_mgmt_process();
        
        /* Update audio levels (every 50ms) */
        if (current_time - last_audio_update >= 50) {
            last_audio_update = current_time;
            audio_mgmt_process();
        }
        
        /* Update UI (every 50ms for 20fps) */
        if (current_time - last_ui_update >= 50) {
            last_ui_update = current_time;
            ui_mgmt_update();
        }
        
        /* Handle system events */
        Handle_System_Events();
        
        /* Short delay to prevent busy waiting */
        HAL_Delay(1);
    }
}

/**
 * @brief Handle system events (button presses, etc.)
 */
static void Handle_System_Events(void)
{
    /* Check for button events */
    button_event_t power_event = button_adapter_interface.poll_button_event(BUTTON_POWER);
    
    switch (power_event) {
        case BUTTON_EVENT_SHORT_PRESS:
            /* Toggle audio mode */
            audio_mgmt_toggle_mode();
            break;
            
        case BUTTON_EVENT_LONG_PRESS:
            /* Request shutdown */
            power_mgmt_request_shutdown();
            break;
            
        default:
            /* No action */
            break;
    }
    
    /* Check for low power conditions */
    battery_status_t battery = power_mgmt_get_battery_status();
    if (battery.is_critical_voltage) {
        ui_mgmt_show_shutdown_sequence();
    } else if (battery.is_low_voltage) {
        ui_mgmt_show_low_battery_warning();
    }
}

/**
 * @brief System Clock Configuration
 */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    /* Configure the main internal regulator output voltage */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    
    /* Initialize the RCC Oscillators according to the specified parameters */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI14;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.HSI14CalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    
    /* Initialize the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief GPIO Initialization
 */
static void GPIO_Init(void)
{
    /* Enable GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /* Initialize all configured pins */
    GPIO_Init_All();
}

/**
 * @brief SPI1 Initialization (for display)
 */
static void SPI1_Init(void)
{
    /* SPI1 parameter configuration */
    SPI_HandleTypeDef hspi1;
    
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;
    hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        Error_Handler();
    }
    
    /* Initialize SPI GPIO */
    SPI_GPIO_Init();
}

/**
 * @brief TIM2 Initialization (for PWM backlight)
 */
static void TIM2_Init(void)
{
    TIM_HandleTypeDef htim2;
    TIM_OC_InitTypeDef sConfigOC = {0};
    
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 47;  /* 48MHz / 48 = 1MHz */
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 1000 - 1;  /* 1kHz PWM */
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
        Error_Handler();
    }
    
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 500;  /* 50% duty cycle */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }
    
    /* Start PWM */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    
    /* Initialize Timer GPIO */
    Timer_GPIO_Init();
}

/**
 * @brief ADC Initialization (for battery monitoring)
 */
static void ADC_Init(void)
{
    ADC_HandleTypeDef hadc;
    ADC_ChannelConfTypeDef sConfig = {0};
    
    /* Common config */
    hadc.Instance = ADC1;
    hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc.Init.Resolution = ADC_RESOLUTION_12B;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
    hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc.Init.LowPowerAutoWait = DISABLE;
    hadc.Init.LowPowerAutoPowerOff = DISABLE;
    hadc.Init.ContinuousConvMode = DISABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc.Init.DMAContinuousRequests = DISABLE;
    hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
    
    if (HAL_ADC_Init(&hadc) != HAL_OK) {
        Error_Handler();
    }
    
    /* Configure for battery voltage channel */
    sConfig.Channel = BATTERY_ADC_CHANNEL;
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    
    /* Initialize ADC GPIO */
    ADC_GPIO_Init();
}

/**
 * @brief Error Handler
 */
void Error_Handler(void)
{
    /* Disable interrupts */
    __disable_irq();
    
    /* Flash power LED to indicate error */
    while (1) {
        HAL_GPIO_TogglePin(POWER_LED_Port, POWER_LED_Pin);
        HAL_Delay(100);
    }
}