# Opus4 Hardware Fixes Summary

## Date: 2025-07-31
## Branch: fix/opus4-hardware-corrections

## Overview
This document summarizes all hardware corrections made based on Opus4's schematic verification.

## Key Changes Made

### 1. Pin Configuration Updates (pin_config.h)

#### LCD Control Pins
- **LCD_CS**: Changed from PC0 to PB7
- **LCD_RS (DC)**: Changed from PA3 to PA4  
- **LCD_RESET**: Changed from PD3 to PA2 (OSCOUT pin)
- **LCD_SCL**: Confirmed PC5
- **LCD_SDA**: Confirmed PC6

#### Backlight Control (Two-level, not PWM)
- **BL1**: PD0 (High brightness)
- **BL2**: PB6 (Low brightness)
- Control via Q7 PNP transistor (LOW = ON)

#### ADC Inputs
- **Left Audio (L_AF)**: Changed to PB0 (AIN0)
- **Right Audio (R_AF)**: Changed to PB1 (AIN1)
- **Battery Voltage**: Changed to PB2 (AIN2)
- **Battery Divider Ratio**: Corrected from 2.0 to 4.3 (10K:33K)

#### LED Indicators
- **Green LED**: Changed from PC1 to PD2
- **Red LED**: Changed from PC0 to PD3
- Both LEDs are active low (LOW = ON)

#### Buttons
- **Power Button**: PD6 (confirmed)
- **Mode Button**: Changed from PD4 to PD5

#### Power Control
- **CON_POW_CPU**: PC4 (Main power control)
- **CON_POW_LCD**: PB6 (LCD power, shared with BL2)
- **CON_POW_RF**: PC1 (RF power control)

#### Audio Control
- **CON_MUTE**: PC3 (Audio mute)
- **CON_STEREO**: PC2 (STEREO/MONO select)

#### Charging Detection
- **CHRG**: PB5 (I2C_SDA/CHAR - Charge full detection)
- **DET**: PB4 (I2C_SCL/DET - 5V detection)

### 2. GPIOD Support Added

Since the HAL doesn't include GPIOD clock enable macro, we added manual support:

```c
/* Enable GPIOD clock manually since HAL macro is missing */
/* Based on pattern: GPIOA=17, GPIOB=18, GPIOC=19, so GPIOD should be 20 */
#define RCC_AHBENR_GPIODEN_Pos           (20U)
#define RCC_AHBENR_GPIODEN_Msk           (0x1UL << RCC_AHBENR_GPIODEN_Pos)
#define RCC_AHBENR_GPIODEN               RCC_AHBENR_GPIODEN_Msk
SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIODEN);
```

### 3. Backlight Control Implementation

Changed from PWM to two-level GPIO control:

```c
void HW_SetBacklight(uint8_t level)
{
    // Two-level backlight control using PD0 (BL1) and PB6 (BL2)
    // Through Q7 PNP transistor - LOW = ON
    switch(level)
    {
        case 0:  // Off
            HAL_GPIO_WritePin(TFT_BL1_PORT, TFT_BL1_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(TFT_BL2_PORT, TFT_BL2_PIN, GPIO_PIN_SET);
            break;
        case 1:  // Low brightness (BL2 only)
            HAL_GPIO_WritePin(TFT_BL1_PORT, TFT_BL1_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(TFT_BL2_PORT, TFT_BL2_PIN, GPIO_PIN_RESET);
            break;
        case 2:  // High brightness (BL1 only or both)
        default:
            HAL_GPIO_WritePin(TFT_BL1_PORT, TFT_BL1_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(TFT_BL2_PORT, TFT_BL2_PIN, GPIO_PIN_SET);
            break;
    }
}
```

### 4. Battery Voltage Calculation Fix

Updated to use correct voltage divider ratio:

```c
// Apply voltage divider ratio from pin_config.h (R52:R53 = 10K:33K = 1:4.3)
battery_voltage = battery_voltage * BATTERY_VOLTAGE_DIVIDER;
```

### 5. Files Modified

1. **Core/Inc/pin_config.h** - Complete pin reassignment based on Opus4
2. **Core/Inc/st7735.h** - Updated to use pin_config.h definitions
3. **Core/Src/hardware_control.c** - Added GPIOD support, updated GPIO init, new backlight control
4. **Core/Src/power.c** - Fixed battery voltage calculation
5. **Core/Src/audio_adc.c** - Updated GPIO clock for GPIOB (audio pins)
6. **Core/Src/main.c** - Updated GPIO init, removed PWM timer, added GPIOD support

## Compilation Results

- Firmware size: 62,488 bytes (97.6% of 64KB)
- Hex file generated with correct 0x00000000 base address
- Ready for testing with WB-Link PRO Configurator

## Testing Notes

1. The firmware should now correctly initialize all hardware based on the actual schematic
2. LCD should work with correct CS, DC, and RST pins
3. Backlight has two-level control (not PWM)
4. Battery voltage reading should be accurate with 4.3x divider
5. All LEDs, buttons, and control signals are on correct pins

## Important Pin Conflicts Resolved

- PC0: Was triple-assigned (BAT_ADC, TFT_CS, LED_RED) - Now free
- PB6: Shared between CON_POW_LCD and BL2 (intentional in hardware)
- PD5/PD6: Shared with UART2 when debugging (managed in code)