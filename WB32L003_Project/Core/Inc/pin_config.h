/**
 * @file pin_config.h
 * @brief Pin configuration for WB32L003 MCU according to o3 schematic analysis
 * @version 3.0 - Updated based on o3's hardware verification
 */

#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wb32l003.h"

/* ========== LCD INTERFACE (LH096NT) ========== */
/* Based on o3 analysis: LCD uses PB7-PB0 pins sequentially */
/* Using bit-banged SPI on PB5/PB3 instead of hardware SPI */
#define LCD_RST_PIN             GPIO_PIN_7      // PB7 - LCD_RESET
#define LCD_RST_PORT            GPIOB
#define LCD_RS_PIN              GPIO_PIN_6      // PB6 - LCD_RS (Data/Command)
#define LCD_RS_PORT             GPIOB
#define LCD_SCL_PIN             GPIO_PIN_5      // PB5 - LCD_SCL (SPI Clock)
#define LCD_SCL_PORT            GPIOB
#define LCD_CS_PIN              GPIO_PIN_4      // PB4 - LCD_CS (Chip Select)
#define LCD_CS_PORT             GPIOB
#define LCD_SDA_PIN             GPIO_PIN_3      // PB3 - LCD_SDA (SPI Data)
#define LCD_SDA_PORT            GPIOB

/* Backlight control - Two level brightness via Q7 transistor */
#define CLD_BL2_PIN             GPIO_PIN_2      // PB2 - CLD_BL2 (Low brightness)
#define CLD_BL2_PORT            GPIOB
#define CLD_BL1_PIN             GPIO_PIN_1      // PB1 - CLD_BL1 (High brightness via Q7)
#define CLD_BL1_PORT            GPIOB

/* For compatibility with existing code */
#define TFT_CS_PIN              LCD_CS_PIN
#define TFT_CS_PORT             LCD_CS_PORT
#define TFT_DC_PIN              LCD_RS_PIN
#define TFT_DC_PORT             LCD_RS_PORT
#define TFT_RST_PIN             LCD_RST_PIN
#define TFT_RST_PORT            LCD_RST_PORT
#define TFT_SPI_SCLK_PIN        LCD_SCL_PIN
#define TFT_SPI_SCLK_PORT       LCD_SCL_PORT
#define TFT_SPI_MOSI_PIN        LCD_SDA_PIN
#define TFT_SPI_MOSI_PORT       LCD_SDA_PORT

/* ========== ADC INPUTS ========== */
/* Battery voltage detection - o3 verified */
#define BAT_ADC_PIN             GPIO_PIN_2      // PC2 - V_Batt (with 1:2 divider R3/R4=47K)
#define BAT_ADC_PORT            GPIOC
#define BAT_ADC_CHANNEL         ADC_CHANNEL_13

/* Audio level detection ADC inputs - o3 verified */
#define L_AD_PIN                GPIO_PIN_4      // PC4 - AF_L (Left channel)
#define L_AD_PORT               GPIOC
#define L_AD_ADC_CHANNEL        ADC_CHANNEL_11

#define R_AD_PIN                GPIO_PIN_3      // PC3 - AF_R (Right channel)
#define R_AD_PORT               GPIOC
#define R_AD_ADC_CHANNEL        ADC_CHANNEL_12

/* ========== USER INTERFACE ========== */
/* Buttons - o3: Only power button exists, connected to PB0 */
#define SW_POW_PIN              GPIO_PIN_0      // PB0 - SW_POW (Power button detection)
#define SW_POW_PORT             GPIOB

/* LED indicators - o3 verified */
#define LED_RED_PIN             GPIO_PIN_0      // PC0 - Red LED (Common anode, LOW = ON)
#define LED_RED_PORT            GPIOC
#define LED_GREEN_PIN           GPIO_PIN_1      // PC1 - Green LED (Common anode, LOW = ON)
#define LED_GREEN_PORT          GPIOC

/* ========== AUDIO AND CONTROL PINS ========== */
/* Audio control - o3 verified */
#define CON_MUTE_PIN            GPIO_PIN_4      // PA4 - CON_MUTE (Low = mute, High = unmute)
#define CON_MUTE_PORT           GPIOA

#define CON_STEREO_PIN          GPIO_PIN_3      // PD3 - CON_STEREO (Low = stereo, High = mono)
#define CON_STEREO_PORT         GPIOD

/* Power management pins - o3 verified */
#define CON_POW_CPU_PIN         GPIO_PIN_6      // PC6 - Main power control (via Q5 PNP)
#define CON_POW_CPU_PORT        GPIOC

#define CON_POW_RF_PIN          GPIO_PIN_5      // PC5 - 5V power control (for audio amp)
#define CON_POW_RF_PORT         GPIOC

#define CON_POW_LCD_PIN         GPIO_PIN_3      // PA3 - LCD power control
#define CON_POW_LCD_PORT        GPIOA

/* Charging detection - o3 verified */
#define DET_PIN                 GPIO_PIN_7      // PD7 - DET (5V detection via divider)
#define DET_PORT                GPIOD

#define CHAR_PIN                GPIO_PIN_6      // PD6 - CHAR (Charge full detection from TP4056)
#define CHAR_PORT               GPIOD

/* ========== ISP PROGRAMMING INTERFACE ========== */
/* o3: Using ISP interface, not SWD/UART */
#define ISP_CK_PIN              GPIO_PIN_7      // PC7 - ISP Clock
#define ISP_CK_PORT             GPIOC
#define ISP_D_PIN               GPIO_PIN_7      // PC7 - ISP Data (shared pin)
#define ISP_D_PORT              GPIOC

/* ========== UNUSED PINS (o3 verified) ========== */
/* PD0, PD4, PD5, PB5 are not connected */

/* Backlight control method */
#define BACKLIGHT_USE_TRANSISTOR 1  // Using Q7 transistor control

/* Battery voltage divider ratio - o3 verified */
#define BATTERY_VOLTAGE_DIVIDER 2.0f  // R3/R4 = 47K/47K = 1:2 divider

/* Battery voltage thresholds */
#define BATTERY_VOLTAGE_WARNING 3.0f  // Red LED flash threshold
#define BATTERY_VOLTAGE_CRITICAL 2.8f // Auto shutdown threshold

/* ========== COMPATIBILITY DEFINITIONS ========== */
/* For existing code compatibility */
#define KEY_PWR_PIN             SW_POW_PIN
#define KEY_PWR_PORT            SW_POW_PORT
#define MUTE_PIN                CON_MUTE_PIN
#define MUTE_PORT               CON_MUTE_PORT
#define TFT_BL1_PIN             CLD_BL1_PIN
#define TFT_BL1_PORT            CLD_BL1_PORT
#define TFT_BL2_PIN             CLD_BL2_PIN
#define TFT_BL2_PORT            CLD_BL2_PORT
#define CHRG_PIN                CHAR_PIN
#define CHRG_PORT               CHAR_PORT

#ifdef __cplusplus
}
#endif

#endif /* __PIN_CONFIG_H */