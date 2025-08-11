/**
 * @file audio_adc.h
 * @brief Audio ADC sampling for VU meter
 * 
 * This module handles real-time audio level detection
 * with 1kHz sampling rate and interrupt-driven processing
 */

#ifndef __AUDIO_ADC_H
#define __AUDIO_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wb32l003.h"
#include <stdbool.h>

/* Audio sampling configuration */
#define AUDIO_SAMPLE_RATE       1000    // 1kHz sampling rate
#define AUDIO_BUFFER_SIZE       32      // Samples per channel
#define AUDIO_UPDATE_RATE       20      // Hz (50ms update for display)

/* Audio level range */
#define AUDIO_LEVEL_MIN         0
#define AUDIO_LEVEL_MAX         31

/* Audio data structure */
typedef struct {
    uint16_t left_samples[AUDIO_BUFFER_SIZE];
    uint16_t right_samples[AUDIO_BUFFER_SIZE];
    uint8_t buffer_index;
    bool data_ready;
    
    /* Processed levels (0-31) */
    uint8_t left_level;
    uint8_t right_level;
    uint8_t mono_level;  // Average of left and right
    
    /* Peak detection */
    uint16_t left_peak;
    uint16_t right_peak;
} audio_data_t;

/* Function prototypes */
void Audio_ADC_Init(void);
void Audio_ADC_Start(void);
void Audio_ADC_Stop(void);
void Audio_ADC_Process(void);
bool Audio_ADC_IsDataReady(void);
uint8_t Audio_ADC_GetLeftLevel(void);
uint8_t Audio_ADC_GetRightLevel(void);
uint8_t Audio_ADC_GetMonoLevel(void);

/* Timer interrupt handler (to be called from TIM3_IRQHandler) */
void Audio_ADC_TimerCallback(void);

/* ADC conversion complete callback */
void Audio_ADC_ConversionComplete(uint16_t left_value, uint16_t right_value);

#ifdef __cplusplus
}
#endif

#endif /* __AUDIO_ADC_H */