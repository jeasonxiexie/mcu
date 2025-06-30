#include "wb32l003.h"
#include "pin_config.h"
#include "st7735.h"
#include "power.h"
#include "ui.h"
#include "hardware_control.h"

static void SystemClock_Config(void);
static void GPIO_Init(void);
static void SPI_Init(void);
static void ADC_Init(void);
static void TIM_Init(void);

volatile uint32_t tick_ms = 0;
volatile uint8_t frame_update_flag = 0;

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    SPI_Init();
    ADC_Init();
    TIM_Init();
    
    HW_Init();
    UI_Init();
    Power_Init();
    
    // Use proper power-on sequence
    HW_PowerOnSequence();
    
    // Switch to VU meter mode after boot
    UI_DrawBackground();
    UI_SetAudioMode(AUDIO_MODE_STEREO);
    
    // Demo variables for VU meter
    uint8_t demo_left = 0;
    uint8_t demo_right = 0;
    uint8_t demo_direction = 1;
    
    while (1)
    {
        Power_Process();
        
        if (frame_update_flag)
        {
            frame_update_flag = 0;
            
            // Get audio levels from hardware (or demo if not connected)
            uint16_t left_adc = HW_GetLeftAudioLevel();
            uint16_t right_adc = HW_GetRightAudioLevel();
            
            // Convert ADC values (0-4095) to VU meter scale (0-32)
            uint8_t left_level = (left_adc * 32) / 4096;
            uint8_t right_level = (right_adc * 32) / 4096;
            
            UI_UpdateVUMeter(left_level, right_level);
            UI_UpdateAnimation();
        }
        
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
        {
            HAL_Delay(50);  // Debounce
            if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
            {
                uint32_t press_time = 0;
                // Wait for button release or timeout
                while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
                {
                    HAL_Delay(10);
                    press_time += 10;
                    if (press_time > 3000)  // Long press > 3s
                    {
                        Power_Off();
                        break;
                    }
                }
                
                // If short press (< 3s), toggle audio mode
                if (press_time < 3000 && press_time > 0)
                {
                    // Toggle between STEREO and MONO
                    static audio_mode_t current_audio_mode = AUDIO_MODE_STEREO;
                    current_audio_mode = (current_audio_mode == AUDIO_MODE_STEREO) ? 
                                       AUDIO_MODE_MONO : AUDIO_MODE_STEREO;
                    UI_SetAudioMode(current_audio_mode);
                    
                    // Update hardware audio mode
                    HW_SetAudioMode(current_audio_mode == AUDIO_MODE_MONO);
                    
                    // TODO: Save audio mode to flash/EEPROM for power-off memory
                    // SaveAudioModeToFlash(current_audio_mode);
                }
            }
        }
        
        if (Power_GetMode() == POWER_MODE_STANDBY)
        {
            Power_EnterStandby();
        }
    }
}

static void SystemClock_Config(void)
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

static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_SET);
}

static void SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    SPI_HandleTypeDef hspi1 = {0};
    
    __HAL_RCC_SPI1_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    HAL_SPI_Init(&hspi1);
}

static void ADC_Init(void)
{
    ADC_HandleTypeDef hadc;
    ADC_ChannelConfTypeDef sConfig = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_ADC_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    hadc.Instance = ADC1;
    hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc.Init.Resolution = ADC_RESOLUTION_12B;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc.Init.LowPowerAutoWait = DISABLE;
    hadc.Init.LowPowerAutoPowerOff = DISABLE;
    hadc.Init.ContinuousConvMode = DISABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc.Init.DMAContinuousRequests = DISABLE;
    hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
    HAL_ADC_Init(&hadc);
    
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc, &sConfig);
}

static void TIM_Init(void)
{
    TIM_HandleTypeDef htim2;
    TIM_OC_InitTypeDef sConfigOC = {0};
    
    __HAL_RCC_TIM2_CLK_ENABLE();
    
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 31;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 999;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&htim2);
    
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 500;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);
    
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}

void HAL_SYSTICK_Callback(void)
{
    tick_ms++;
    
    if (tick_ms % 50 == 0)
    {
        frame_update_flag = 1;
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}