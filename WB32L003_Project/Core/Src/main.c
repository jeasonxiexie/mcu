#include "wb32l003.h"
#include "pin_config.h"
#include "st7735.h"
#include "power.h"
#include "ui.h"
#include "hardware_control.h"
#include "audio_adc.h"

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
    
    // Start audio ADC sampling
    Audio_ADC_Start();
    
    // Switch to VU meter mode after boot
    UI_DrawBackground();
    UI_SetAudioMode(AUDIO_MODE_STEREO);
    
    while (1)
    {
        Power_Process();
        
        // Process audio ADC data  
        Audio_ADC_Process();
        
        if (frame_update_flag)
        {
            frame_update_flag = 0;
            
            // Get real audio levels from ADC
            uint8_t left_level = Audio_ADC_GetLeftLevel();
            uint8_t right_level = Audio_ADC_GetRightLevel();
            
            // In MONO mode, use the combined level
            if (HW_GetAudioMode() == AUDIO_MODE_MONO)
            {
                uint8_t mono_level = Audio_ADC_GetMonoLevel();
                left_level = mono_level;
                right_level = mono_level;
            }
            
            UI_UpdateVUMeter(left_level, right_level);
            UI_UpdateAnimation();
        }
        
        /* Check power button */
        if (HAL_GPIO_ReadPin(KEY_PWR_PORT, KEY_PWR_PIN) == GPIO_PIN_RESET)
        {
            HAL_Delay(50);  // Debounce
            if (HAL_GPIO_ReadPin(KEY_PWR_PORT, KEY_PWR_PIN) == GPIO_PIN_RESET)
            {
                uint32_t press_time = 0;
                // Wait for button release or timeout
                while (HAL_GPIO_ReadPin(KEY_PWR_PORT, KEY_PWR_PIN) == GPIO_PIN_RESET)
                {
                    HAL_Delay(10);
                    press_time += 10;
                    if (press_time > 3000)  // Long press > 3s
                    {
                        Power_Off();
                        break;
                    }
                }
                // Power button short press could cycle backlight levels
            }
        }
        
        /* Check mode button */
        if (HAL_GPIO_ReadPin(KEY_MODE_PORT, KEY_MODE_PIN) == GPIO_PIN_RESET)
        {
            HAL_Delay(50);  // Debounce
            if (HAL_GPIO_ReadPin(KEY_MODE_PORT, KEY_MODE_PIN) == GPIO_PIN_RESET)
            {
                // Wait for button release
                while (HAL_GPIO_ReadPin(KEY_MODE_PORT, KEY_MODE_PIN) == GPIO_PIN_RESET)
                {
                    HAL_Delay(10);
                }
                
                // Toggle between STEREO and MONO
                static audio_mode_t current_audio_mode = AUDIO_MODE_STEREO;
                current_audio_mode = (current_audio_mode == AUDIO_MODE_STEREO) ? 
                                   AUDIO_MODE_MONO : AUDIO_MODE_STEREO;
                UI_SetAudioMode(current_audio_mode);
                
                // Update hardware audio mode
                HW_SetAudioMode(current_audio_mode == AUDIO_MODE_MONO);
                
                // Update MODE_OUT pin
                HAL_GPIO_WritePin(MODE_OUT_PORT, MODE_OUT_PIN, 
                                  current_audio_mode == AUDIO_MODE_MONO ? GPIO_PIN_SET : GPIO_PIN_RESET);
                
                // TODO: Save audio mode to flash for power-off memory
                // SaveAudioModeToFlash(current_audio_mode);
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
    
    /* Enable all GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /* TFT control pins */
    GPIO_InitStruct.Pin = TFT_CS_PIN;  // PB4
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TFT_CS_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TFT_DC_PIN;  // PA3
    HAL_GPIO_Init(TFT_DC_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TFT_RST_PIN;  // PD3
    HAL_GPIO_Init(TFT_RST_PORT, &GPIO_InitStruct);
    
    /* Set initial states */
    HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);    // CS high (inactive)
    HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);    // DC high
    HAL_GPIO_WritePin(TFT_RST_PORT, TFT_RST_PIN, GPIO_PIN_SET);  // RST high
    
    /* PWM backlight pin - will be configured as AF in TIM_Init */
    GPIO_InitStruct.Pin = TFT_BL_PIN;  // PA1
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;  // TIM2_CH2
    HAL_GPIO_Init(TFT_BL_PORT, &GPIO_InitStruct);
    
    /* Button inputs with pull-up */
    GPIO_InitStruct.Pin = KEY_PWR_PIN;  // PD6
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY_PWR_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = KEY_MODE_PIN;  // PD4
    HAL_GPIO_Init(KEY_MODE_PORT, &GPIO_InitStruct);
    
    /* LED output */
    GPIO_InitStruct.Pin = LED_RED_PIN;  // PD5
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_RED_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);  // LED off (active low)
    
    /* Mode output pin */
    GPIO_InitStruct.Pin = MODE_OUT_PIN;  // PB2
    HAL_GPIO_Init(MODE_OUT_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(MODE_OUT_PORT, MODE_OUT_PIN, GPIO_PIN_RESET);  // Default STEREO
    
    /* ADC pins will be configured as analog in ADC_Init */
    /* SPI pins will be configured as AF in SPI_Init */
}

static void SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    SPI_HandleTypeDef hspi1 = {0};
    
    __HAL_RCC_SPI1_CLK_ENABLE();
    
    /* Configure SPI pins: PC5 (SCK) and PC6 (MOSI) */
    GPIO_InitStruct.Pin = TFT_SPI_SCLK_PIN | TFT_SPI_MOSI_PIN;  // PC5 | PC6
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;  // SPI1 alternate function
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /* Configure SPI peripheral */
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;   // CPOL = 0
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;       // CPHA = 0 (Mode 0)
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;  // 24MHz/2 = 12MHz
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
    
    /* Configure ADC GPIO pins as analog */
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    
    /* Battery ADC pin - PC0 */
    GPIO_InitStruct.Pin = BAT_ADC_PIN;
    HAL_GPIO_Init(BAT_ADC_PORT, &GPIO_InitStruct);
    
    /* Audio ADC pins - PC1, PC2 */
    GPIO_InitStruct.Pin = L_AD_PIN;
    HAL_GPIO_Init(L_AD_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = R_AD_PIN;
    HAL_GPIO_Init(R_AD_PORT, &GPIO_InitStruct);
    
    /* Configure ADC for multi-channel scan mode */
    hadc.Instance = ADC1;
    hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc.Init.Resolution = ADC_RESOLUTION_12B;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode = ADC_SCAN_ENABLE;  // Enable scan mode for multiple channels
    hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;  // End of sequence conversion
    hadc.Init.LowPowerAutoWait = DISABLE;
    hadc.Init.LowPowerAutoPowerOff = DISABLE;
    hadc.Init.ContinuousConvMode = DISABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;  // Software trigger for now
    hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc.Init.DMAContinuousRequests = ENABLE;  // Enable DMA continuous requests
    hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    HAL_ADC_Init(&hadc);
    
    /* Configure Battery voltage channel - PC0 (ADC_IN15) */
    sConfig.Channel = BAT_ADC_CHANNEL;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;  // Long sampling for accuracy
    HAL_ADC_ConfigChannel(&hadc, &sConfig);
    
    /* Configure Left audio channel - PC1 (ADC_IN14) */
    sConfig.Channel = L_AD_ADC_CHANNEL;
    sConfig.Rank = 2;
    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;  // Faster for audio
    HAL_ADC_ConfigChannel(&hadc, &sConfig);
    
    /* Configure Right audio channel - PC2 (ADC_IN13) */
    sConfig.Channel = R_AD_ADC_CHANNEL;
    sConfig.Rank = 3;
    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;  // Faster for audio
    HAL_ADC_ConfigChannel(&hadc, &sConfig);
    
    /* Calibrate ADC */
    HAL_ADCEx_Calibration_Start(&hadc);
}

static void TIM_Init(void)
{
    TIM_HandleTypeDef htim2;  // Use TIM2 for PWM as in original code
    TIM_OC_InitTypeDef sConfigOC = {0};
    
    /* TIM2 for PWM backlight control */
    __HAL_RCC_TIM2_CLK_ENABLE();
    
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 31;  // Similar to original
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 999;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&htim2);
    
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 500;  // 50% duty cycle default
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);
    
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    
    /* Note: ADC will use software trigger for now
       TIM3 could be added later for hardware trigger if needed */
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