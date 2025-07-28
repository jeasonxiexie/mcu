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
/* Using PC5/PC6 for SPI as per WB32L003 datasheet */
#define TFT_SPI_SCLK_PIN        GPIO_PIN_5      // PC5 - SPI_SCK (High Speed)
#define TFT_SPI_SCLK_PORT       GPIOC
#define TFT_SPI_MOSI_PIN        GPIO_PIN_6      // PC6 - SPI_MOSI (High Speed)
#define TFT_SPI_MOSI_PORT       GPIOC

/* TFT control pins */
#define TFT_CS_PIN              GPIO_PIN_0      // PC0 - Chip Select (Low active)
#define TFT_CS_PORT             GPIOC
#define TFT_DC_PIN              GPIO_PIN_3      // PA3 - Data/Command
#define TFT_DC_PORT             GPIOA
#define TFT_RST_PIN             GPIO_PIN_3      // PD3 - Hardware Reset
#define TFT_RST_PORT            GPIOD

/* TFT backlight control - Using TIM2 (TIM1 not supported in current HAL) */
#define TFT_BL_PIN              GPIO_PIN_1      // PA1 - TIM2_CH2 PWM backlight
#define TFT_BL_PORT             GPIOA
#define TFT_BL_TIM              TIM2
#define TFT_BL_CHANNEL          TIM_CHANNEL_2

/* ========== ADC INPUTS ========== */
/* Battery voltage detection */
#define BAT_ADC_PIN             GPIO_PIN_0      // PC0 - ADC_IN15 (with 1/2 divider)
#define BAT_ADC_PORT            GPIOC
#define BAT_ADC_CHANNEL         ADC_CHANNEL_15

/* Audio level detection ADC inputs - For VU meter display */
#define L_AD_PIN                GPIO_PIN_1      // PC1 - ADC_IN14 (Left channel)
#define L_AD_PORT               GPIOC
#define L_AD_ADC_CHANNEL        ADC_CHANNEL_14

#define R_AD_PIN                GPIO_PIN_2      // PC2 - ADC_IN13 (Right channel)
#define R_AD_PORT               GPIOC
#define R_AD_ADC_CHANNEL        ADC_CHANNEL_13

/* ========== USER INTERFACE ========== */
/* Buttons */
#define KEY_PWR_PIN             GPIO_PIN_6      // PD6 - Power button (EXTI)
#define KEY_PWR_PORT            GPIOD

#define KEY_MODE_PIN            GPIO_PIN_4      // PD4 - Mode button (STEREO/MONO)
#define KEY_MODE_PORT           GPIOD

/* LED indicators */
#define LED_RED_PIN             GPIO_PIN_0      // PB0 - Red LED for low battery
#define LED_RED_PORT            GPIOB
#define LED_GREEN_PIN           GPIO_PIN_2      // PB2 - Green LED (additional)
#define LED_GREEN_PORT          GPIOB

/* ========== AUDIO AND CONTROL PINS ========== */
/* Mode output pin */
#define MODE_OUT_PIN            GPIO_PIN_2      // PB2 - Output selected mode
#define MODE_OUT_PORT           GPIOB

/* Audio control - Pins to be assigned based on final PCB */
#define MUTE_PIN                GPIO_PIN_4      // PA4 - Audio mute control (High = unmute)
#define MUTE_PORT               GPIOA

#define V2_PIN                  GPIO_PIN_6      // PB6 - STEREO/MONO select (High = MONO)
#define V2_PORT                 GPIOB

/* Power management pins - To be confirmed with PCB design */
#define CON_POW_PIN             GPIO_PIN_7      // PB7 - Main power control
#define CON_POW_PORT            GPIOB

#define CON_LCD_PIN             GPIO_PIN_8      // PB8 - LCD power control
#define CON_LCD_PORT            GPIOB

/* LED indicators - To be confirmed */
#define GREEN_PIN               GPIO_PIN_3      // PB3 - Green LED (Low = on)
#define GREEN_PORT              GPIOB

#define RED_PIN                 GPIO_PIN_4      // PB4 - Red LED (Low = on)
#define RED_PORT                GPIOB

/* Charging detection */
#define CHRG_PIN                GPIO_PIN_5      // PB5 - Charging status (Low = charging)
#define CHRG_PORT               GPIOB

/* ========== DEBUG UART ========== */
/* Using PB6/PB7 to avoid conflict with SPI on PC6/PC7 */
#define DEBUG_UART_TX_PIN       GPIO_PIN_6      // PB6 - UART TX
#define DEBUG_UART_TX_PORT      GPIOB
#define DEBUG_UART_RX_PIN       GPIO_PIN_6      // PD6 - UART RX (shared with KEY_PWR)
#define DEBUG_UART_RX_PORT      GPIOD

/* Backlight levels (mA) */
#define BACKLIGHT_LEVEL_OFF     0
#define BACKLIGHT_LEVEL_LOW     5   // 5mA
#define BACKLIGHT_LEVEL_MID     12  // 12mA
#define BACKLIGHT_LEVEL_HIGH    20  // 20mA

/* Battery voltage thresholds */
#define BATTERY_VOLTAGE_WARNING 3.0f  // Red LED flash threshold
#define BATTERY_VOLTAGE_CRITICAL 2.8f // Auto shutdown threshold

#ifdef __cplusplus
}
#endif

#endif /* __PIN_CONFIG_H */