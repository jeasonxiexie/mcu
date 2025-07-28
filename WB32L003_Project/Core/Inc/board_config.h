/**
 * @file board_config.h
 * @brief Board-level configuration for WB32L003 Earphone Amplifier Project
 * @version 1.0
 * @date 2025-07-01
 * 
 * This file centralizes all hardware configuration for easy maintenance
 * and portability. Update this file when changing MCU or board layout.
 */

#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wb32l003.h"
#include "pin_config.h"

/* ========== MCU CONFIGURATION ========== */
#define MCU_MODEL               "WB32L003"
#define MCU_CORE                "Cortex-M0+"
#define MCU_FLASH_SIZE          (64 * 1024)    // 64KB
#define MCU_SRAM_SIZE           (4 * 1024)     // 4KB
#define MCU_CLOCK_FREQ          24000000UL      // 24MHz HSI

/* ========== SYSTEM CLOCK CONFIGURATION ========== */
#define SYSTEM_CLOCK_HSI        1               // Use internal HSI
#define SYSTEM_CLOCK_FREQ       24000000UL      // 24MHz
#define FLASH_WAIT_STATES       0               // 0 wait states at 24MHz

/* ========== SPI CONFIGURATION ========== */
#define SPI_INSTANCE            SPI1
#define SPI_BAUDRATE_DIV        SPI_BAUDRATEPRESCALER_2  // 12MHz for display
#define SPI_USE_DMA             1               // Enable DMA for better performance
#define SPI_DMA_CHANNEL         DMA1_Channel3   // TX channel

/* ========== ADC CONFIGURATION ========== */
#define ADC_INSTANCE            ADC1
#define ADC_SAMPLE_RATE         1000            // 1kHz for audio
#define ADC_RESOLUTION          ADC_RESOLUTION_12B
#define ADC_USE_DMA             1
#define ADC_DMA_CHANNEL         DMA1_Channel1

/* ADC channel assignments (verified from datasheet) */
#define ADC_CHANNEL_BATTERY     ADC_CHANNEL_15  // PC0
#define ADC_CHANNEL_AUDIO_L     ADC_CHANNEL_14  // PC1
#define ADC_CHANNEL_AUDIO_R     ADC_CHANNEL_13  // PC2
#define ADC_CHANNEL_COUNT       3

/* ========== TIMER CONFIGURATION ========== */
/* TIM1 for PWM backlight control */
#define PWM_TIMER               TIM1
#define PWM_FREQUENCY           1000            // 1kHz PWM
#define PWM_RESOLUTION          1000            // 0.1% resolution

/* TIM3 for ADC trigger */
#define ADC_TRIGGER_TIMER       TIM3
#define ADC_TRIGGER_FREQ        1000            // 1kHz sampling

/* ========== UART DEBUG CONFIGURATION ========== */
#define DEBUG_UART              UART1
#define DEBUG_UART_BAUDRATE     115200
#define DEBUG_UART_AF           GPIO_AF1_UART1

/* ========== POWER MANAGEMENT ========== */
#define IDLE_TIMEOUT_MS         (20 * 1000)     // 20 seconds
#define SLEEP_TIMEOUT_MS        (10 * 60 * 1000) // 10 minutes
#define BATTERY_ADC_DIVIDER     2               // 1/2 voltage divider
#define BATTERY_WARNING_V       3.0f            // Red LED flash threshold
#define BATTERY_CRITICAL_V      2.8f            // Auto shutdown threshold
#define BATTERY_FULL_V          4.2f            // Full charge voltage

/* ========== DISPLAY CONFIGURATION ========== */
#define DISPLAY_WIDTH           80
#define DISPLAY_HEIGHT          160
#define DISPLAY_DRIVER          "ST7735S"
#define DISPLAY_SPI_MODE        0               // Mode 0: CPOL=0, CPHA=0
#define DISPLAY_BACKLIGHT_STEPS 3               // Off, Low, Mid, High

/* ========== AUDIO CONFIGURATION ========== */
#define AUDIO_BUFFER_SIZE       32              // Samples per channel
#define AUDIO_UPDATE_RATE       20              // Hz (50ms update)
#define AUDIO_LEVEL_MAX         31              // VU meter max level

/* ========== FLASH STORAGE ========== */
#define FLASH_PAGE_SIZE         1024            // 1KB per page
#define FLASH_USER_START_ADDR   0x0000F800      // Last page (63KB offset)
#define FLASH_SETTINGS_MAGIC    0x5A5A5A5A      // Magic number for valid data

/* Settings structure stored in flash */
typedef struct {
    uint32_t magic;             // Magic number for validation
    uint8_t audio_mode;         // 0=STEREO, 1=MONO
    uint8_t backlight_level;    // 0-3
    uint8_t reserved[2];        // Padding for alignment
    uint32_t crc;               // CRC-16 checksum
} flash_settings_t;

/* ========== PIN ASSIGNMENT STATUS ========== */
/*
 * CONFIRMED PINS (from latest schematic):
 * - SPI: PC5(SCK), PC6(MOSI), PC0(CS), PA3(DC), PD3(RST)
 * - PWM: PB1(TIM1_CH1)
 * - ADC: PC0(BAT), PC1(L_AD), PC2(R_AD)
 * - BTN: PD6(PWR), PD4(MODE)
 * - LED: PB0(RED), PB2(GREEN)
 * - OUT: PB2(MODE_OUT)
 * - DBG: PB6(TX), PD6(RX)
 * 
 * IMPORTANT NOTES:
 * - PC0 is shared between TFT_CS and BAT_ADC
 *   Hardware must ensure proper isolation (>100kΩ divider)
 * - PD6 is shared between KEY_PWR and UART_RX
 *   Software must handle mutual exclusion
 * - PB2 is shared between LED_GREEN and MODE_OUT
 *   Check schematic for actual usage
 */

/* ========== FEATURE FLAGS ========== */
#define ENABLE_DEBUG_UART       1
#define ENABLE_DMA              1
#define ENABLE_LOW_POWER        1
#define ENABLE_FLASH_STORAGE    1
#define ENABLE_WATCHDOG         0       // Disable during development

/* ========== ERROR CODES ========== */
#define ERR_OK                  0
#define ERR_TIMEOUT             -1
#define ERR_INVALID_PARAM       -2
#define ERR_HARDWARE_FAULT      -3
#define ERR_FLASH_WRITE         -4
#define ERR_FLASH_ERASE         -5

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_CONFIG_H */