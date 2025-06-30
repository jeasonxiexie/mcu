/**
 * @file pin_config.h
 * @brief Pin configuration according to project_spec.md and TFT096B039 datasheet
 */

#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wb32l003.h"

/* SPI1 pins for TFT display */
#define TFT_SPI_SCLK_PIN        GPIO_PIN_5      // PA5 - SPI1_SCK
#define TFT_SPI_SCLK_PORT       GPIOA
#define TFT_SPI_MOSI_PIN        GPIO_PIN_7      // PA7 - SPI1_MOSI
#define TFT_SPI_MOSI_PORT       GPIOA

/* TFT control pins */
#define TFT_CS_PIN              GPIO_PIN_0      // PB0 - Chip Select
#define TFT_CS_PORT             GPIOB
#define TFT_DC_PIN              GPIO_PIN_1      // PB1 - Data/Command
#define TFT_DC_PORT             GPIOB
#define TFT_RST_PIN             GPIO_PIN_2      // PB2 - Hardware Reset
#define TFT_RST_PORT            GPIOB

/* TFT backlight control */
#define TFT_BL_PIN              GPIO_PIN_1      // PA1 - PWM backlight control
#define TFT_BL_PORT             GPIOA
#define TFT_BL_TIM              TIM2
#define TFT_BL_CHANNEL          TIM_CHANNEL_2

/* Power management */
#define BAT_ADC_PIN             GPIO_PIN_0      // PA0 - Battery voltage detect
#define BAT_ADC_PORT            GPIOA
#define BAT_ADC_CHANNEL         ADC_CHANNEL_0

#define KEY_PWR_PIN             GPIO_PIN_13     // PC13 - Power button
#define KEY_PWR_PORT            GPIOC

/* Status LED (if exists, not in spec) */
#define LED_STATUS_PIN          GPIO_PIN_2      // PA2 - Status LED
#define LED_STATUS_PORT         GPIOA

/* ========== NEW PINS FROM SPECS.MD ========== */
/* Note: The following pins need to be assigned based on your hardware design */

/* Power management pins - Required for proper power sequencing */
// #define CON_POW_PIN             GPIO_PIN_X      // Power control
// #define CON_POW_PORT            GPIOX
// #define CON_LCD_PIN             GPIO_PIN_X      // LCD power control
// #define CON_LCD_PORT            GPIOX

/* Audio control - Required for audio mute during power on/off */
// #define MUTE_PIN                GPIO_PIN_X      // Audio mute control (High = unmute)
// #define MUTE_PORT               GPIOX

/* Charging detection - For battery charging status */
// #define CHRG_PIN                GPIO_PIN_X      // Charging status (Low = charging)
// #define CHRG_PORT               GPIOX

/* Audio level detection ADC inputs - For VU meter display */
// #define L_AD_PIN                GPIO_PIN_X      // Left channel audio detect
// #define L_AD_PORT               GPIOX
// #define L_AD_ADC_CHANNEL        ADC_CHANNEL_X
// #define R_AD_PIN                GPIO_PIN_X      // Right channel audio detect
// #define R_AD_PORT               GPIOX
// #define R_AD_ADC_CHANNEL        ADC_CHANNEL_X

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