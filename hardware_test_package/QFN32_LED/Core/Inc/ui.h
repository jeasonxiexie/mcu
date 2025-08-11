#ifndef __UI_H
#define __UI_H

#include "wb32l003.h"
#include "power.h"
#include "ui_assets.h"

#define METER_FRAME_COUNT 12
#define METER_FPS 20
#define METER_FRAME_DELAY (1000 / METER_FPS)

typedef enum {
    UI_MODE_BOOT = 0,
    UI_MODE_VU_METER,
    UI_MODE_SHUTDOWN
} ui_mode_t;

typedef enum {
    AUDIO_MODE_STEREO = 0,
    AUDIO_MODE_MONO
} audio_mode_t;

void UI_Init(void);
void UI_ShowBootLogo(void);
void UI_ShowOffLogo(void);
void UI_UpdateAnimation(void);
void UI_UpdateBatteryIcon(battery_level_t level);
void UI_SetAudioMode(audio_mode_t mode);
void UI_UpdateVUMeter(uint8_t left_level, uint8_t right_level);
void UI_DrawBackground(void);
void UI_Clear(void);

// Resource data
extern const uint16_t background_base[];
extern const uint16_t mono_text[];
extern const uint16_t stereo_text[];
extern const uint16_t battery_empty[];
extern const uint16_t battery_bar[];
extern const uint16_t battery_low_flash[];
extern const uint16_t pointer_image[];
extern const uint16_t volume_blocks[][160*80];

// Animation frames
extern const uint16_t mono_animation_frames[][160*80];
extern const uint16_t stereo_animation_frames[][160*80];
extern const uint16_t battery_low_animation_frames[][160*80];

#endif