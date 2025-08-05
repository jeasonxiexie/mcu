/**
 * @file hardware_control_fixed.c
 * @brief Fixed hardware control with proper pin conflict resolution
 * @version 8.0 - Resolves PC5/PC6 conflicts
 */

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
    /* CRITICAL: PC6 is shared between LCD_SDA and CON_POW_CPU!
     * We cannot use PC6 for power control while LCD is active.
     * Solution: Keep PC6 as input (high-Z) and rely on external pull-down
     * or use a different approach for power latching.
     */
    
    // DO NOT control PC6 directly - it's used by LCD!
    // This function is now a no-op to prevent conflicts
}

void HW_SetLCDPower(bool enable)
{
    /* PA3 controls LCD via Q7 PNP transistor
     * PA3 LOW -> Q7 ON -> LCD power ON
     * PA3 HIGH -> Q7 OFF -> LCD power OFF
     */
    HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void HW_Set5VPower(bool enable)
{
    /* CRITICAL: PC5 is shared between LCD_SCL and CON_POW_RF!
     * We cannot use PC5 for 5V control while LCD is active.
     * This needs hardware redesign or alternative control method.
     */
    
    // DO NOT control PC5 directly - it's used by LCD clock!
    // This function is now a no-op to prevent conflicts
}

void HW_SetMute(bool mute)
{
    // Note: Logic is inverted - HIGH = unmute, LOW = mute
    HAL_GPIO_WritePin(MUTE_PORT, MUTE_PIN, mute ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void HW_SetAudioMode(bool mono)
{
    /* Hardware team verified: PD2 controls CON_STEREO signal
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
    /* PC1 = Green LED BUT this conflicts with LCD_RS!
     * Cannot use PC1 for LED while LCD is active.
     */
    // Disabled due to pin conflict
}

void HW_SetRedLED(bool on)
{
    /* PC0 = Red LED BUT this conflicts with LCD_CS!
     * Cannot use PC0 for LED while LCD is active.
     */
    // Disabled due to pin conflict
}

bool HW_IsCharging(void)
{
    /* PD7 = DET (5V detection) */
    return (HAL_GPIO_ReadPin(DET_PORT, DET_PIN) == GPIO_PIN_SET);
}

bool HW_IsChargeFull(void)
{
    /* PD6 = CHAR from TP4056 STDBY# pin */
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
    #ifndef __HAL_RCC_GPIOD_CLK_ENABLE
        #define RCC_AHBENR_GPIODEN_Pos           (20U)
        #define RCC_AHBENR_GPIODEN_Msk           (0x1UL << RCC_AHBENR_GPIODEN_Pos)
        #define RCC_AHBENR_GPIODEN               RCC_AHBENR_GPIODEN_Msk
        SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIODEN);
        volatile uint32_t tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_GPIODEN);
        (void)tmpreg;
    #else
        __HAL_RCC_GPIOD_CLK_ENABLE();
    #endif
    
    /* Configure output pins */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    /* LCD control pins - NOW CORRECTLY ON GPIOC! */
    /* Note: PC5 (SCL) and PC6 (SDA) will be controlled by LCD driver */
    /* PC0 (CS), PC1 (RS), PC3 (RST) can be initialized here */
    GPIO_InitStruct.Pin = LCD_RST_PIN | LCD_RS_PIN | LCD_CS_PIN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /* Note: PC5 and PC6 will be initialized by ST7735_Init() */
    
    /* Backlight control pins */
    GPIO_InitStruct.Pin = CLD_BL1_PIN | CLD_BL2_PIN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_SET);  // BL1 OFF
    HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_SET);  // BL2 OFF
    
    /* MUTE control - PA4 */
    GPIO_InitStruct.Pin = CON_MUTE_PIN;
    HAL_GPIO_Init(CON_MUTE_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_MUTE_PORT, CON_MUTE_PIN, GPIO_PIN_RESET);  // Start muted
    
    /* STEREO/MONO control - PD2 */
    GPIO_InitStruct.Pin = CON_STEREO_PIN;
    HAL_GPIO_Init(CON_STEREO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_STEREO_PORT, CON_STEREO_PIN, GPIO_PIN_RESET);  // Start in STEREO
    
    /* LCD power control - PA3 */
    GPIO_InitStruct.Pin = CON_POW_LCD_PIN;
    HAL_GPIO_Init(CON_POW_LCD_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, GPIO_PIN_SET); // LCD OFF initially (HIGH = OFF)
    
    /* Configure input pins */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    
    /* Power button detection - PB0 */
    GPIO_InitStruct.Pin = SW_POW_PIN;
    HAL_GPIO_Init(SW_POW_PORT, &GPIO_InitStruct);
    
    /* Mode button - PD3 */
    GPIO_InitStruct.Pin = SW_MODE_PIN;
    HAL_GPIO_Init(SW_MODE_PORT, &GPIO_InitStruct);
    
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
    /* Modified power-on sequence to avoid pin conflicts:
     * 1. Cannot use PC6 for power latch (conflicts with LCD_SDA)
     * 2. Cannot use PC5 for 5V control (conflicts with LCD_SCL)
     * 3. Cannot use PC0/PC1 for LEDs (conflicts with LCD_CS/LCD_RS)
     */
    
    // 1. Mute audio during power on
    HW_SetMute(true);  // PA4 LOW
    
    // 2. Enable LCD power
    HW_SetLCDPower(true);  // PA3 LOW (inverted logic)
    HAL_Delay(50);  // Wait for power to stabilize
    
    // 3. Initialize LCD
    ST7735_Init();
    
    // 4. Enable backlight
    HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_RESET);  // BL1 ON
    HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_RESET);  // BL2 ON (bright)
    
    // 5. Show boot logo
    UI_ShowBootLogo();
    
    // 6. Unmute audio after everything is stable
    HAL_Delay(100);
    HW_SetMute(false);  // PA4 HIGH
    
    // Note: Cannot show battery status with LEDs due to pin conflicts
    
    HAL_Delay(1000);  // Show boot logo
}

void HW_PowerOffSequence(void)
{
    /* Modified power-off sequence */
    
    // 1. Mute audio before power off
    HW_SetMute(true);  // PA4 LOW
    HAL_Delay(10);
    
    // 2. Turn off backlight
    HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_SET);  // BL1 OFF
    HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_SET);  // BL2 OFF
    HAL_Delay(10);
    
    // 3. Disable LCD power
    HW_SetLCDPower(false);  // PA3 HIGH (inverted logic)
    HAL_Delay(10);
    
    // 4. Cannot cut main power via PC6 (conflicts with LCD)
    // System will need external power control or hardware redesign
    
    // Enter low power mode
    __WFI();  // Wait for interrupt
}

// PD6 pin multiplexing management
static bool pd6_is_uart = false;

void HW_ConfigurePD6AsButton(void)
{
    if (!pd6_is_uart) return;
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = KEY_PWR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY_PWR_PORT, &GPIO_InitStruct);
    
    pd6_is_uart = false;
}

void HW_ConfigurePD6AsUART(void)
{
    if (pd6_is_uart) return;
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = CHAR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(CHAR_PORT, &GPIO_InitStruct);
    
    pd6_is_uart = true;
}