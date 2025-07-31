/**
 * @file pin_config.h
 * @brief Pin configuration for WB32L003 MCU according to new PinMap
 * @version 2.0 - Updated for WB32L003 with PC5/PC6 SPI
 */

#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wb32l003.h"

/* ========== SPI INTERFACE FOR TFT DISPLAY ========== */
/* Based on Opus4 schematic verification */
#define TFT_SPI_SCLK_PIN        GPIO_PIN_5      // PC5 - LCD_SCL
#define TFT_SPI_SCLK_PORT       GPIOC
#define TFT_SPI_MOSI_PIN        GPIO_PIN_6      // PC6 - LCD_SDA
#define TFT_SPI_MOSI_PORT       GPIOC

/* TFT control pins - Corrected based on actual schematic */
#define TFT_CS_PIN              GPIO_PIN_7      // PB7 - LCD_CS (not PC0!)
#define TFT_CS_PORT             GPIOB
#define TFT_DC_PIN              GPIO_PIN_4      // PA4 - LCD_RS (not PA3!)
#define TFT_DC_PORT             GPIOA
#define TFT_RST_PIN             GPIO_PIN_2      // PA2 - LCD_RESET (OSCOUT pin)
#define TFT_RST_PORT            GPIOA

/* TFT backlight control - Two-level brightness, not PWM */
#define TFT_BL1_PIN             GPIO_PIN_0      // PD0 - CLD_BL1 (High brightness)
#define TFT_BL1_PORT            GPIOD
#define TFT_BL2_PIN             GPIO_PIN_6      // PB6 - CLD_BL2 (Low brightness)
#define TFT_BL2_PORT            GPIOB

/* ========== ADC INPUTS ========== */
/* Based on Opus4 verification - Using PB0/PB1/PB2 for ADC */
/* Battery voltage detection */
#define BAT_ADC_PIN             GPIO_PIN_2      // PB2 - AIN2 (V_Batt with 1:4.3 divider)
#define BAT_ADC_PORT            GPIOB
#define BAT_ADC_CHANNEL         ADC_CHANNEL_2

/* Audio level detection ADC inputs - For VU meter display */
#define L_AD_PIN                GPIO_PIN_0      // PB0 - AIN0 (Left channel - L_AF)
#define L_AD_PORT               GPIOB
#define L_AD_ADC_CHANNEL        ADC_CHANNEL_0

#define R_AD_PIN                GPIO_PIN_1      // PB1 - AIN1 (Right channel - R_AF)
#define R_AD_PORT               GPIOB
#define R_AD_ADC_CHANNEL        ADC_CHANNEL_1

/* ========== USER INTERFACE ========== */
/* Buttons */
#define KEY_PWR_PIN             GPIO_PIN_6      // PD6 - SW_POW (Power button)
#define KEY_PWR_PORT            GPIOD

#define KEY_MODE_PIN            GPIO_PIN_5      // PD5 - KEY_STEREO (Mode button) - Corrected!
#define KEY_MODE_PORT           GPIOD

/* LED indicators - Dual-color LED L-C175JRJGCT-AM */
#define LED_RED_PIN             GPIO_PIN_3      // PD3 - Red LED (Corrected from PC0)
#define LED_RED_PORT            GPIOD
#define LED_GREEN_PIN           GPIO_PIN_2      // PD2 - Green LED (Corrected from PC1)
#define LED_GREEN_PORT          GPIOD

/* ========== AUDIO AND CONTROL PINS ========== */
/* Audio control - Based on Opus4 verification */
#define MUTE_PIN                GPIO_PIN_3      // PC3 - CON_MUTE (Audio mute control)
#define MUTE_PORT               GPIOC

#define CON_STEREO_PIN          GPIO_PIN_2      // PC2 - CON_STEREO (STEREO/MONO select)
#define CON_STEREO_PORT         GPIOC

/* Power management pins - Based on Opus4 verification */
#define CON_POW_CPU_PIN         GPIO_PIN_4      // PC4 - Main power control
#define CON_POW_CPU_PORT        GPIOC

#define CON_POW_LCD_PIN         GPIO_PIN_6      // PB6 - LCD power control (shared with BL2)
#define CON_POW_LCD_PORT        GPIOB

#define CON_POW_RF_PIN          GPIO_PIN_1      // PC1 - RF power control
#define CON_POW_RF_PORT         GPIOC

/* Charging detection */
#define CHRG_PIN                GPIO_PIN_5      // PB5 - I2C_SDA/CHAR (Charge full detection)
#define CHRG_PORT               GPIOB

#define DET_PIN                 GPIO_PIN_4      // PB4 - I2C_SCL/DET (5V detection)
#define DET_PORT                GPIOB

/* ========== DEBUG UART ========== */
/* Based on Opus4 verification */
#define DEBUG_UART_TX_PIN       GPIO_PIN_5      // PD5 - UART2_TX (shared with KEY_MODE)
#define DEBUG_UART_TX_PORT      GPIOD
#define DEBUG_UART_RX_PIN       GPIO_PIN_6      // PD6 - UART2_RX (shared with KEY_PWR)
#define DEBUG_UART_RX_PORT      GPIOD

/* Backlight levels (mA) */
#define BACKLIGHT_LEVEL_OFF     0
#define BACKLIGHT_LEVEL_LOW     5   // 5mA
#define BACKLIGHT_LEVEL_MID     12  // 12mA
#define BACKLIGHT_LEVEL_HIGH    20  // 20mA

/* Battery voltage thresholds */
#define BATTERY_VOLTAGE_WARNING 3.0f  // Red LED flash threshold
#define BATTERY_VOLTAGE_CRITICAL 2.8f // Auto shutdown threshold

/* Battery voltage divider ratio - Based on R52/R53 (10K:33K) */
#define BATTERY_VOLTAGE_DIVIDER 4.3f  // Corrected from 2.0f

#ifdef __cplusplus
}
#endif

#endif /* __PIN_CONFIG_H */