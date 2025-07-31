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
    /* o3 verified: PC6 controls Q5 PNP transistor via Q6 NPN
     * PC6 LOW -> Q6 OFF -> Q5 base pulled LOW by SW_POW -> Q5 ON -> Power ON
     * PC6 HIGH -> Q6 ON -> Q5 base HIGH -> Q5 OFF -> Power OFF
     * So logic is INVERTED: LOW = Power ON, HIGH = Power OFF
     */
    HAL_GPIO_WritePin(CON_POW_CPU_PORT, CON_POW_CPU_PIN, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void HW_SetLCDPower(bool enable)
{
    /* o3 verified: PA3 directly controls LCD power */
    HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void HW_Set5VPower(bool enable)
{
    /* o3 verified: PC5 controls 5V boost for audio amplifier */
    HAL_GPIO_WritePin(CON_POW_RF_PORT, CON_POW_RF_PIN, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void HW_SetMute(bool mute)
{
    // Note: Logic is inverted - HIGH = unmute, LOW = mute
    HAL_GPIO_WritePin(MUTE_PORT, MUTE_PIN, mute ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void HW_SetAudioMode(bool mono)
{
    /* o3 verified: PD3 controls CON_STEREO signal to U4 analog switch
     * LOW = STEREO, HIGH = MONO
     */
    HAL_GPIO_WritePin(CON_STEREO_PORT, CON_STEREO_PIN, mono ? GPIO_PIN_SET : GPIO_PIN_RESET);
    
    current_audio_mode = mono ? AUDIO_MODE_MONO : AUDIO_MODE_STEREO;
}

audio_mode_t HW_GetAudioMode(void)
{
    return current_audio_mode;
}

void HW_SetGreenLED(bool on)
{
    /* o3 verified: PC1 = Green LED, common anode, LOW = ON */
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, on ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void HW_SetRedLED(bool on)
{
    /* o3 verified: PC0 = Red LED, common anode, LOW = ON */
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, on ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

bool HW_IsCharging(void)
{
    /* o3 verified: PD7 = DET (5V detection), PD6 = CHAR (charge full) */
    /* Check if 5V is present (charging) */
    return (HAL_GPIO_ReadPin(DET_PORT, DET_PIN) == GPIO_PIN_SET);
}

bool HW_IsChargeFull(void)
{
    /* o3 verified: PD6 = CHAR from TP4056 STDBY# pin */
    /* LOW = charge complete */
    return (HAL_GPIO_ReadPin(CHAR_PORT, CHAR_PIN) == GPIO_PIN_RESET);
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
    
    /* Configure output pins */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    /* LCD control pins (PB7-PB1) */
    GPIO_InitStruct.Pin = LCD_RST_PIN | LCD_RS_PIN | LCD_SCL_PIN | LCD_CS_PIN | LCD_SDA_PIN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* Backlight control pins */
    GPIO_InitStruct.Pin = CLD_BL1_PIN | CLD_BL2_PIN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_SET);  // BL1 OFF
    HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_SET);  // BL2 OFF
    
    /* MUTE control - PA4 */
    GPIO_InitStruct.Pin = CON_MUTE_PIN;
    HAL_GPIO_Init(CON_MUTE_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_MUTE_PORT, CON_MUTE_PIN, GPIO_PIN_RESET);  // Start muted
    
    /* STEREO/MONO control - PD3 */
    GPIO_InitStruct.Pin = CON_STEREO_PIN;
    HAL_GPIO_Init(CON_STEREO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_STEREO_PORT, CON_STEREO_PIN, GPIO_PIN_RESET);  // Start in STEREO
    
    /* Power control pins */
    GPIO_InitStruct.Pin = CON_POW_CPU_PIN;  // PC6
    HAL_GPIO_Init(CON_POW_CPU_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_POW_CPU_PORT, CON_POW_CPU_PIN, GPIO_PIN_RESET);  // Keep power ON
    
    GPIO_InitStruct.Pin = CON_POW_RF_PIN;   // PC5
    HAL_GPIO_Init(CON_POW_RF_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_POW_RF_PORT, CON_POW_RF_PIN, GPIO_PIN_SET);    // 5V OFF initially
    
    GPIO_InitStruct.Pin = CON_POW_LCD_PIN;  // PA3
    HAL_GPIO_Init(CON_POW_LCD_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, GPIO_PIN_RESET); // LCD OFF initially
    
    /* LED indicators */
    GPIO_InitStruct.Pin = LED_RED_PIN;  // PC0
    HAL_GPIO_Init(LED_RED_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);  // LED off
    
    GPIO_InitStruct.Pin = LED_GREEN_PIN;  // PC1
    HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);  // LED off
    
    /* Configure input pins */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    
    /* Power button detection - PB0 */
    GPIO_InitStruct.Pin = SW_POW_PIN;
    HAL_GPIO_Init(SW_POW_PORT, &GPIO_InitStruct);
    
    /* Charging detection - PD6/PD7 */
    GPIO_InitStruct.Pin = DET_PIN;  // PD7
    HAL_GPIO_Init(DET_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = CHAR_PIN;  // PD6
    HAL_GPIO_Init(CHAR_PORT, &GPIO_InitStruct);
    
    /* Initialize audio ADC system */
    Audio_ADC_Init();
}

void HW_PowerOnSequence(void)
{
    /* o3 verified power-on sequence:
     * 1. MCU must immediately set PC6 LOW to maintain power after SW_POW release
     * 2. Enable LCD power, then 5V for audio
     * 3. Initialize display and audio
     */
    
    // 1. Ensure main power stays ON (PC6 must be LOW)
    HW_SetMainPower(true);  // This sets PC6 LOW
    HAL_Delay(10);
    
    // 2. Enable LCD power
    HW_SetLCDPower(true);
    HAL_Delay(10);
    
    // 3. Initialize LCD
    ST7735_Init();
    ST7735_SetBacklight(2);  // High brightness for boot
    UI_ShowBootLogo();
    
    // 4. Enable 5V for audio amplifier
    HW_Set5VPower(true);
    HAL_Delay(50);  // Allow 5V to stabilize
    
    // 5. Unmute audio
    HW_SetMute(false);
    
    // 6. Set initial LED state based on battery
    float voltage = Power_GetBatteryVoltage();
    if (voltage >= 3.0f)
    {
        HW_SetGreenLED(true);
        HW_SetRedLED(false);
    }
    else
    {
        HW_SetGreenLED(false);
        HW_SetRedLED(true);
    }
    
    HAL_Delay(1000);  // Show boot logo
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
    GPIO_InitStruct.Pin = CHAR_PIN;  // PD6 - same pin as CHAR
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;  // USART1 alternate function
    HAL_GPIO_Init(CHAR_PORT, &GPIO_InitStruct);
    
    pd6_is_uart = true;
}