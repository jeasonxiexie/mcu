/**
 * @file pin_config_corrected.h
 * @brief CORRECTED Pin configuration based on actual schematic PDF
 * @version 4.0 - Major corrections based on schematic analysis
 * 
 * ⚠️ CRITICAL: Previous pin definitions were WRONG!
 * LCD pins are on GPIOC, not GPIOB!
 */

#ifndef __PIN_CONFIG_CORRECTED_H
#define __PIN_CONFIG_CORRECTED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wb32l003.h"

/* ========== LCD INTERFACE - MAJOR CORRECTION! ========== */
/* All LCD pins are on GPIOC, NOT GPIOB! */
#define LCD_SCL_PIN             GPIO_PIN_5      // PC5 - LCD_SCL (SPI Clock)
#define LCD_SCL_PORT            GPIOC           // ⚠️ Was wrongly GPIOB
#define LCD_SDA_PIN             GPIO_PIN_6      // PC6 - LCD_SDA (SPI Data/MOSI)
#define LCD_SDA_PORT            GPIOC           // ⚠️ Was wrongly GPIOB
#define LCD_CS_PIN              GPIO_PIN_0      // PC0 - LCD_CS (Chip Select)
#define LCD_CS_PORT             GPIOC           // ⚠️ Was wrongly GPIOB
#define LCD_RS_PIN              GPIO_PIN_1      // PC1 - LCD_RS (Data/Command)
#define LCD_RS_PORT             GPIOC           // ⚠️ Was wrongly GPIOB
#define LCD_RST_PIN             GPIO_PIN_3      // PC3 - LCD_RESET
#define LCD_RST_PORT            GPIOC           // ⚠️ Was wrongly GPIOB

/* Backlight control - These are correct on GPIOB */
#define CLD_BL1_PIN             GPIO_PIN_1      // PB1 - High brightness (LOW = ON via Q7 PNP)
#define CLD_BL1_PORT            GPIOB
#define CLD_BL2_PIN             GPIO_PIN_2      // PB2 - Low brightness (HIGH = ON)
#define CLD_BL2_PORT            GPIOB

/* ========== LED INDICATORS - CORRECTED! ========== */
/* LEDs are on GPIOB, not GPIOC! Common anode, LOW = ON */
#define LED_RED_PIN             GPIO_PIN_0      // PB0 - Red LED
#define LED_RED_PORT            GPIOB           // ⚠️ Was wrongly GPIOC
#define LED_GREEN_PIN           GPIO_PIN_1      // PB1 - Green LED (shares with BL1!)
#define LED_GREEN_PORT          GPIOB           // ⚠️ Was wrongly GPIOC

/* ========== CRITICAL PIN CONFLICTS! ========== */
/* 
 * ⚠️ WARNING: Pin conflicts discovered:
 * - PB0: Red LED + Power button detection
 * - PB1: Green LED + Backlight high brightness control
 * - PC5: LCD_SCL + CON_POW_RF (audio amp power)
 * - PC6: LCD_SDA + CON_POW_CPU (main power latch)
 * 
 * This explains why the system doesn't work properly!
 */

/* ========== POWER MANAGEMENT ========== */
#define SW_POW_PIN              GPIO_PIN_0      // PB0 - Power button (shares with LED_RED!)
#define SW_POW_PORT             GPIOB
#define CON_POW_CPU_PIN         GPIO_PIN_6      // PC6 - Power latch (conflicts with LCD_SDA!)
#define CON_POW_CPU_PORT        GPIOC
#define CON_POW_RF_PIN          GPIO_PIN_5      // PC5 - Audio power (conflicts with LCD_SCL!)
#define CON_POW_RF_PORT         GPIOC

/* ========== OTHER PINS ========== */
/* Mode button */
#define SW_MODE_PIN             GPIO_PIN_3      // PD3 - Mode button
#define SW_MODE_PORT            GPIOD

/* Audio control */
#define CON_MUTE_PIN            GPIO_PIN_4      // PA4 - Mute control
#define CON_MUTE_PORT           GPIOA
#define CON_STEREO_PIN          GPIO_PIN_2      // PD2 - Stereo/Mono
#define CON_STEREO_PORT         GPIOD

/* ADC inputs */
#define BAT_ADC_PIN             GPIO_PIN_2      // PC2 - Battery voltage
#define BAT_ADC_PORT            GPIOC
#define L_AD_PIN                GPIO_PIN_4      // PC4 - Left audio
#define L_AD_PORT               GPIOC
#define R_AD_PIN                GPIO_PIN_3      // PC3 - Right audio
#define R_AD_PORT               GPIOC

/* USB/Charging detection */
#define DET_PIN                 GPIO_PIN_7      // PD7 - USB detection
#define DET_PORT                GPIOD
#define CHAR_PIN                GPIO_PIN_6      // PD6 - Charge status
#define CHAR_PORT               GPIOD

/* Programming interface */
#define ISP_CK_PIN              GPIO_PIN_1      // PD1 - ISP Clock (SWD)
#define ISP_CK_PORT             GPIOD
#define ISP_D_PIN               GPIO_PIN_7      // PC7 - ISP Data
#define ISP_D_PORT              GPIOC

/* UART2 for debug */
#define UART2_TX_PIN            GPIO_PIN_5      // PD5
#define UART2_TX_PORT           GPIOD
#define UART2_RX_PIN            GPIO_PIN_6      // PD6
#define UART2_RX_PORT           GPIOD

/* ========== COMPATIBILITY MACROS ========== */
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

#ifdef __cplusplus
}
#endif

#endif /* __PIN_CONFIG_CORRECTED_H */