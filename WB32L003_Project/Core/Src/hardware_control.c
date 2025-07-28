#include "hardware_control.h"
#include "st7735.h"
#include "ui.h"
#include "power.h"
#include "audio_adc.h"
#include "pin_config.h"

// Static variable to track current audio mode
static audio_mode_t current_audio_mode = AUDIO_MODE_STEREO;

void HW_SetMainPower(bool enable)
{
    HAL_GPIO_WritePin(CON_POW_PORT, CON_POW_PIN, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void HW_SetLCDPower(bool enable)
{
    HAL_GPIO_WritePin(CON_LCD_PORT, CON_LCD_PIN, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void HW_SetMute(bool mute)
{
    // Note: Logic is inverted - HIGH = unmute, LOW = mute
    HAL_GPIO_WritePin(MUTE_PORT, MUTE_PIN, mute ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void HW_SetAudioMode(bool mono)
{
    // V2 pin: LOW = STEREO, HIGH = MONO (if available)
    #ifdef V2_PIN
    HAL_GPIO_WritePin(V2_PORT, V2_PIN, mono ? GPIO_PIN_SET : GPIO_PIN_RESET);
    #endif
    
    // MODE_OUT pin: mirrors the selected mode
    HAL_GPIO_WritePin(MODE_OUT_PORT, MODE_OUT_PIN, mono ? GPIO_PIN_SET : GPIO_PIN_RESET);
    
    current_audio_mode = mono ? AUDIO_MODE_MONO : AUDIO_MODE_STEREO;
}

audio_mode_t HW_GetAudioMode(void)
{
    return current_audio_mode;
}

void HW_SetGreenLED(bool on)
{
    // Note: GREEN_PIN might not be available in new design
    // This function kept for compatibility, but may not be used
    #ifdef GREEN_PIN
    HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, on ? GPIO_PIN_RESET : GPIO_PIN_SET);
    #endif
}

void HW_SetRedLED(bool on)
{
    // Using LED_RED_PIN from new PinMap (PD5)
    // Note: Logic might be inverted - verify with hardware
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, on ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

bool HW_IsCharging(void)
{
    // LOW = charging
    return (HAL_GPIO_ReadPin(CHRG_PORT, CHRG_PIN) == GPIO_PIN_RESET);
}

uint16_t HW_GetLeftAudioLevel(void)
{
    // Return processed audio level (0-31) scaled to ADC range (0-4095)
    uint8_t level = Audio_ADC_GetLeftLevel();
    return (level * 4095) / 31;
}

uint16_t HW_GetRightAudioLevel(void)
{
    // Return processed audio level (0-31) scaled to ADC range (0-4095)
    uint8_t level = Audio_ADC_GetRightLevel();
    return (level * 4095) / 31;
}

void HW_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Enable GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /* Configure output pins */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    /* MUTE control (if available) */
    #ifdef MUTE_PIN
    GPIO_InitStruct.Pin = MUTE_PIN;
    HAL_GPIO_Init(MUTE_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(MUTE_PORT, MUTE_PIN, GPIO_PIN_RESET);  // Start muted
    #endif
    
    /* V2 (STEREO/MONO select) - if available */
    #ifdef V2_PIN
    GPIO_InitStruct.Pin = V2_PIN;
    HAL_GPIO_Init(V2_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(V2_PORT, V2_PIN, GPIO_PIN_RESET);  // Start in STEREO
    #endif
    
    /* MODE_OUT pin - outputs current mode */
    GPIO_InitStruct.Pin = MODE_OUT_PIN;
    HAL_GPIO_Init(MODE_OUT_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(MODE_OUT_PORT, MODE_OUT_PIN, GPIO_PIN_RESET);  // Start in STEREO
    
    /* Power control pins (if available) */
    #ifdef CON_POW_PIN
    GPIO_InitStruct.Pin = CON_POW_PIN;
    HAL_GPIO_Init(CON_POW_PORT, &GPIO_InitStruct);
    #endif
    
    #ifdef CON_LCD_PIN
    GPIO_InitStruct.Pin = CON_LCD_PIN;
    HAL_GPIO_Init(CON_LCD_PORT, &GPIO_InitStruct);
    #endif
    
    /* LED indicators */
    #ifdef GREEN_PIN
    GPIO_InitStruct.Pin = GREEN_PIN;
    HAL_GPIO_Init(GREEN_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GREEN_PORT, GREEN_PIN, GPIO_PIN_SET);  // LED off
    #endif
    
    /* Red LED (new pin on PD5) */
    GPIO_InitStruct.Pin = LED_RED_PIN;
    HAL_GPIO_Init(LED_RED_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);  // LED off
    
    /* Configure input pins */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    
    /* Add Green LED support for new design */
    #ifdef LED_GREEN_PIN
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin = LED_GREEN_PIN;
    HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);  // LED off
    #endif
    
    /* Charging detection (if available) */
    #ifdef CHRG_PIN
    GPIO_InitStruct.Pin = CHRG_PIN;
    HAL_GPIO_Init(CHRG_PORT, &GPIO_InitStruct);
    #endif
    
    /* Initialize audio ADC system */
    Audio_ADC_Init();
}

void HW_PowerOnSequence(void)
{
    // Implement power-on sequence as per spec
    // 1. Set CON_POW and CON_LCD high
    HW_SetMainPower(true);
    HW_SetLCDPower(true);
    HAL_Delay(10);
    
    // 2. Initialize and show boot screen
    ST7735_Init();
    UI_ShowBootLogo();
    HAL_Delay(1000);
    
    // 3. Unmute audio
    HW_SetMute(false);
    
    // 4. Set initial LED state
    float voltage = Power_GetBatteryVoltage();
    if (voltage >= 3.0f)
    {
        HW_SetGreenLED(true);
        HW_SetRedLED(false);
    }
}

void HW_PowerOffSequence(void)
{
    // Implement power-off sequence as per spec
    // 1. Mute audio
    HW_SetMute(true);
    HAL_Delay(10);
    
    // 2. Show shutdown screen
    UI_ShowOffLogo();
    HAL_Delay(1000);
    
    // 3. Set CON_POW and CON_LCD high (maintain power for controlled shutdown)
    HW_SetMainPower(true);
    HW_SetLCDPower(true);
    HAL_Delay(10);
    
    // 4. Enter low power mode
    Power_EnterStandby();
}

// PD6 pin multiplexing management
// PD6 is shared between KEY_PWR and UART_RX
static bool pd6_is_uart = false;

void HW_ConfigurePD6AsButton(void)
{
    if (!pd6_is_uart) return;  // Already configured as button
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Configure as input with pull-up for button
    GPIO_InitStruct.Pin = KEY_PWR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY_PWR_PORT, &GPIO_InitStruct);
    
    pd6_is_uart = false;
}

void HW_ConfigurePD6AsUART(void)
{
    if (pd6_is_uart) return;  // Already configured as UART
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Configure as alternate function for UART RX
    GPIO_InitStruct.Pin = DEBUG_UART_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;  // USART1 alternate function
    HAL_GPIO_Init(DEBUG_UART_RX_PORT, &GPIO_InitStruct);
    
    pd6_is_uart = true;
}