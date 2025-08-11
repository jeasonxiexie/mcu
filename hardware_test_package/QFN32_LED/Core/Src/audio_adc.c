/**
 * @file audio_adc.c
 * @brief Audio ADC sampling implementation
 */

#include "audio_adc.h"
#include "pin_config.h"

/* External error handler */
extern void Error_Handler(void);

/* Private variables */
static audio_data_t audio_data = {0};
static ADC_HandleTypeDef hadc;
static TIM_HandleTypeDef htim3;
static volatile bool adc_busy = false;
static volatile uint16_t adc_counter = 0;

/* Private function prototypes */
static void Audio_ADC_GPIO_Init(void);
static void Audio_ADC_Timer_Init(void);
static void Audio_ADC_Config_Init(void);
static uint8_t Audio_ADC_CalculateLevel(uint16_t peak_value);

/**
 * @brief Initialize audio ADC system
 */
void Audio_ADC_Init(void)
{
    /* Initialize data structure */
    for (int i = 0; i < sizeof(audio_data); i++) {
        ((uint8_t*)&audio_data)[i] = 0;
    }
    
    /* Initialize GPIO pins for ADC */
    Audio_ADC_GPIO_Init();
    
    /* Initialize ADC peripheral */
    Audio_ADC_Config_Init();
    
    /* Initialize timer for 1kHz sampling */
    Audio_ADC_Timer_Init();
}

/**
 * @brief Initialize GPIO pins for audio ADC
 */
static void Audio_ADC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Enable GPIO clock */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /* Configure L_AD pin (PC1) */
    GPIO_InitStruct.Pin = L_AD_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(L_AD_PORT, &GPIO_InitStruct);
    
    /* Configure R_AD pin (PC2) */
    GPIO_InitStruct.Pin = R_AD_PIN;
    HAL_GPIO_Init(R_AD_PORT, &GPIO_InitStruct);
}

/**
 * @brief Initialize ADC for audio sampling
 */
static void Audio_ADC_Config_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    
    /* Enable ADC clock */
    __HAL_RCC_ADC_CLK_ENABLE();
    
    /* Configure ADC */
    hadc.Instance = ADC1;
    hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
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
    
    if (HAL_ADC_Init(&hadc) != HAL_OK)
    {
        Error_Handler();
    }
    
    /* Configure ADC channels */
    /* Left channel */
    sConfig.Channel = L_AD_ADC_CHANNEL;
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;  // Fast sampling
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief Initialize timer for 1kHz sampling rate
 */
static void Audio_ADC_Timer_Init(void)
{
    /* For now, we'll use a simpler approach without timer interrupt
       The main loop will handle sampling timing */
    
    /* TODO: Implement proper timer interrupt when TIM3 is available */
    
    /* Initialize sampling counter */
    adc_counter = 0;
}

/**
 * @brief Start audio ADC sampling
 */
void Audio_ADC_Start(void)
{
    /* Reset counter */
    adc_counter = 0;
    audio_data.data_ready = false;
}

/**
 * @brief Stop audio ADC sampling
 */
void Audio_ADC_Stop(void)
{
    /* Clear data ready flag */
    audio_data.data_ready = false;
}

/**
 * @brief Timer callback for 1kHz sampling
 */
void Audio_ADC_TimerCallback(void)
{
    if (!adc_busy)
    {
        adc_busy = true;
        
        ADC_ChannelConfTypeDef sConfig = {0};
        
        /* Sample left channel */
        sConfig.Channel = L_AD_ADC_CHANNEL;
        sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
        sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
        HAL_ADC_ConfigChannel(&hadc, &sConfig);
        
        HAL_ADC_Start(&hadc);
        if (HAL_ADC_PollForConversion(&hadc, 1) == HAL_OK)
        {
            uint16_t left_value = HAL_ADC_GetValue(&hadc);
            
            /* Sample right channel */
            sConfig.Channel = R_AD_ADC_CHANNEL;
            HAL_ADC_ConfigChannel(&hadc, &sConfig);
            
            HAL_ADC_Start(&hadc);
            if (HAL_ADC_PollForConversion(&hadc, 1) == HAL_OK)
            {
                uint16_t right_value = HAL_ADC_GetValue(&hadc);
                
                /* Process the samples */
                Audio_ADC_ConversionComplete(left_value, right_value);
            }
        }
        
        adc_busy = false;
    }
}

/**
 * @brief Process ADC conversion results
 */
void Audio_ADC_ConversionComplete(uint16_t left_value, uint16_t right_value)
{
    /* Store samples in circular buffer */
    audio_data.left_samples[audio_data.buffer_index] = left_value;
    audio_data.right_samples[audio_data.buffer_index] = right_value;
    
    /* Advance buffer index */
    audio_data.buffer_index = (audio_data.buffer_index + 1) % AUDIO_BUFFER_SIZE;
    
    /* Mark data as ready when buffer is full */
    static uint8_t sample_count = 0;
    if (++sample_count >= AUDIO_BUFFER_SIZE)
    {
        sample_count = 0;
        audio_data.data_ready = true;
    }
}

/**
 * @brief Process audio data and calculate levels
 */
void Audio_ADC_Process(void)
{
    if (!audio_data.data_ready)
        return;
    
    uint32_t left_sum = 0, right_sum = 0;
    uint16_t left_peak = 0, right_peak = 0;
    
    /* Calculate average and peak values */
    for (int i = 0; i < AUDIO_BUFFER_SIZE; i++)
    {
        /* Remove DC offset (assume 2048 is center for 12-bit ADC) */
        int16_t left_ac = (int16_t)audio_data.left_samples[i] - 2048;
        int16_t right_ac = (int16_t)audio_data.right_samples[i] - 2048;
        
        /* Get absolute values */
        uint16_t left_abs = (left_ac < 0) ? -left_ac : left_ac;
        uint16_t right_abs = (right_ac < 0) ? -right_ac : right_ac;
        
        /* Update sums */
        left_sum += left_abs;
        right_sum += right_abs;
        
        /* Update peaks */
        if (left_abs > left_peak) left_peak = left_abs;
        if (right_abs > right_peak) right_peak = right_abs;
    }
    
    /* Store peak values */
    audio_data.left_peak = left_peak;
    audio_data.right_peak = right_peak;
    
    /* Calculate levels (0-31) based on peak values */
    audio_data.left_level = Audio_ADC_CalculateLevel(left_peak);
    audio_data.right_level = Audio_ADC_CalculateLevel(right_peak);
    audio_data.mono_level = (audio_data.left_level + audio_data.right_level) / 2;
    
    /* Clear data ready flag */
    audio_data.data_ready = false;
}

/**
 * @brief Calculate display level from peak value
 */
static uint8_t Audio_ADC_CalculateLevel(uint16_t peak_value)
{
    /* Map peak value (0-2048) to display level (0-31) */
    /* Apply logarithmic scaling for better visual response */
    
    if (peak_value < 10) return 0;  // Noise floor
    
    /* Simple linear mapping with some compression at high levels */
    uint32_t level = (peak_value * 31) / 2048;
    
    /* Apply some non-linearity for better visual effect */
    if (level > 24)
    {
        level = 24 + ((level - 24) / 2);  // Compress high levels
    }
    
    if (level > 31) level = 31;
    
    return (uint8_t)level;
}

/**
 * @brief Check if new audio data is ready
 */
bool Audio_ADC_IsDataReady(void)
{
    return audio_data.data_ready;
}

/**
 * @brief Get left channel level (0-31)
 */
uint8_t Audio_ADC_GetLeftLevel(void)
{
    return audio_data.left_level;
}

/**
 * @brief Get right channel level (0-31)
 */
uint8_t Audio_ADC_GetRightLevel(void)
{
    return audio_data.right_level;
}

/**
 * @brief Get mono level (0-31)
 */
uint8_t Audio_ADC_GetMonoLevel(void)
{
    return audio_data.mono_level;
}

/* Timer interrupt handler removed - using polling approach for simplicity */