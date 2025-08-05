#include "wb32l003.h"
#include "pin_config.h"
#include "st7735.h"
#include "power.h"
#include "ui.h"
#include "hardware_control.h"
#include "audio_adc.h"

static void SystemClock_Config(void);
static void GPIO_Init(void);
static void ADC_Init(void);
static void TIM_Init(void);

volatile uint32_t tick_ms = 0;
volatile uint8_t frame_update_flag = 0;

int main(void)
{
    /* PC6 already set LOW in Early_PowerLatch() during Reset_Handler */
    /* This ensures power is locked within the critical 150ms window */
    
    HAL_Init();
    SystemClock_Config();
    
    GPIO_Init();
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
        
        /* Check power button - o3 verified: only power button on PB0 */
        if (HAL_GPIO_ReadPin(SW_POW_PORT, SW_POW_PIN) == GPIO_PIN_RESET)
        {
            HAL_Delay(50);  // Debounce
            if (HAL_GPIO_ReadPin(SW_POW_PORT, SW_POW_PIN) == GPIO_PIN_RESET)
            {
                uint32_t press_time = 0;
                // Wait for button release or timeout
                while (HAL_GPIO_ReadPin(SW_POW_PORT, SW_POW_PIN) == GPIO_PIN_RESET)
                {
                    HAL_Delay(10);
                    press_time += 10;
                    if (press_time > 3000)  // Long press > 3s
                    {
                        Power_Off();
                        break;
                    }
                }
                
                if (press_time < 3000)  // Short press - cycle audio mode
                {
                    // Toggle between STEREO and MONO
                    static audio_mode_t current_audio_mode = AUDIO_MODE_STEREO;
                    current_audio_mode = (current_audio_mode == AUDIO_MODE_STEREO) ? 
                                       AUDIO_MODE_MONO : AUDIO_MODE_STEREO;
                    UI_SetAudioMode(current_audio_mode);
                    
                    // Update hardware audio mode
                    HW_SetAudioMode(current_audio_mode == AUDIO_MODE_MONO);
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
    
    /* Enable all GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /* Enable GPIOD clock manually since HAL macro might be missing */
    /* Based on pattern: GPIOA=17, GPIOB=18, GPIOC=19, so GPIOD should be 20 */
    #ifndef __HAL_RCC_GPIOD_CLK_ENABLE
        #define RCC_AHBENR_GPIODEN_Pos           (20U)
        #define RCC_AHBENR_GPIODEN_Msk           (0x1UL << RCC_AHBENR_GPIODEN_Pos)
        #define RCC_AHBENR_GPIODEN               RCC_AHBENR_GPIODEN_Msk
        SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIODEN);
        /* Read back to ensure write completion */
        volatile uint32_t tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_GPIODEN);
        (void)tmpreg;
    #else
        __HAL_RCC_GPIOD_CLK_ENABLE();
    #endif
    
    /* LCD control pins (o3: PB7-PB0 used for LCD) */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    /* LCD pins on GPIOB - software SPI */
    GPIO_InitStruct.Pin = LCD_RST_PIN | LCD_RS_PIN | LCD_SCL_PIN | LCD_CS_PIN | LCD_SDA_PIN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* Backlight control pins */
    GPIO_InitStruct.Pin = CLD_BL1_PIN | CLD_BL2_PIN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* Set initial LCD states */
    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);      // CS high (inactive)
    HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET);      // DC high
    HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);    // RST high
    HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_SET);    // Clock high
    HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_SET);    // BL1 off
    HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_SET);    // BL2 off
    
    /* Power button input (o3: SW_POW on PB0) */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Pin = SW_POW_PIN;
    HAL_GPIO_Init(SW_POW_PORT, &GPIO_InitStruct);
    
    /* Note: All other GPIO initialization is handled by HW_Init() */
    /* including power control, LEDs, audio control, etc. */
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
    
    /* Battery ADC pin - PC2 (o3 verified) */
    GPIO_InitStruct.Pin = BAT_ADC_PIN;
    HAL_GPIO_Init(BAT_ADC_PORT, &GPIO_InitStruct);
    
    /* Audio ADC pins - PC4 (left), PC3 (right) (o3 verified) */
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
    
    /* Configure Battery voltage channel - PC2 (ADC_IN13) (o3 verified) */
    sConfig.Channel = BAT_ADC_CHANNEL;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;  // Long sampling for accuracy
    HAL_ADC_ConfigChannel(&hadc, &sConfig);
    
    /* Configure Left audio channel - PC4 (ADC_IN11) (o3 verified) */
    sConfig.Channel = L_AD_ADC_CHANNEL;
    sConfig.Rank = 2;
    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;  // Faster for audio
    HAL_ADC_ConfigChannel(&hadc, &sConfig);
    
    /* Configure Right audio channel - PC3 (ADC_IN12) (o3 verified) */
    sConfig.Channel = R_AD_ADC_CHANNEL;
    sConfig.Rank = 3;
    sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;  // Faster for audio
    HAL_ADC_ConfigChannel(&hadc, &sConfig);
    
    /* Calibrate ADC */
    HAL_ADCEx_Calibration_Start(&hadc);
    
    /* Additional calibration as per ChatGPT recommendation */
    /* Direct register access for calibration (if HAL doesn't do it) */
    #ifdef ADC_CALR_CAL
    ADC1->CALR = ADC_CALR_CAL;  // Start calibration
    while(ADC1->CALR & ADC_CALR_CAL);  // Wait for calibration to complete
    #endif
}

static void TIM_Init(void)
{
    /* o3 verified: Backlight is controlled via transistor (PB1/PB2), not PWM */
    /* Timer initialization removed - backlight control is handled by HW_SetBacklight */
    
    /* Timers may still be needed for other purposes like ADC triggering */
    /* Add timer initialization here if needed in the future */
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