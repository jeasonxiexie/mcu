#ifndef __UI_ASSETS_H
#define __UI_ASSETS_H

#include "stdint.h"

// Display dimensions
#define UI_DISPLAY_WIDTH  160
#define UI_DISPLAY_HEIGHT 80

// Asset dimensions
#define BATTERY_ICON_WIDTH  20
#define BATTERY_ICON_HEIGHT 10
#define BATTERY_BAR_WIDTH   16
#define BATTERY_BAR_HEIGHT  6

#define MONO_TEXT_WIDTH     40
#define MONO_TEXT_HEIGHT    10
#define STEREO_TEXT_WIDTH   50
#define STEREO_TEXT_HEIGHT  10

#define VOLUME_BLOCK_WIDTH  5
#define VOLUME_BLOCK_HEIGHT 40
#define VOLUME_BLOCKS_COUNT 32

// Background base image (full screen 160x80)
extern const uint16_t ui_background_base[UI_DISPLAY_WIDTH * UI_DISPLAY_HEIGHT];

// Battery icons
extern const uint16_t ui_battery_empty[BATTERY_ICON_WIDTH * BATTERY_ICON_HEIGHT];
extern const uint16_t ui_battery_bar[BATTERY_BAR_WIDTH * BATTERY_BAR_HEIGHT];

// Audio mode text
extern const uint16_t ui_mono_text[MONO_TEXT_WIDTH * MONO_TEXT_HEIGHT];
extern const uint16_t ui_stereo_text[STEREO_TEXT_WIDTH * STEREO_TEXT_HEIGHT];

// Volume level blocks (32 different levels)
extern const uint16_t ui_volume_block_01[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_02[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_03[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_04[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_05[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_06[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_07[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_08[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_09[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_10[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_11[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_12[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_13[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_14[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_15[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_16[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_17[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_18[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_19[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_20[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_21[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_22[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_23[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_24[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_25[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_26[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_27[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_28[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_29[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_30[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_31[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];
extern const uint16_t ui_volume_block_32[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT];

// Array of pointers to volume blocks for easy access
extern const uint16_t* ui_volume_blocks[VOLUME_BLOCKS_COUNT];

// Color definitions in RGB565 format
#define UI_COLOR_BLACK      0x0000
#define UI_COLOR_WHITE      0xFFFF
#define UI_COLOR_RED        0xF800
#define UI_COLOR_GREEN      0x07E0
#define UI_COLOR_BLUE       0x001F
#define UI_COLOR_YELLOW     0xFFE0
#define UI_COLOR_CYAN       0x07FF
#define UI_COLOR_MAGENTA    0xF81F
#define UI_COLOR_ORANGE     0xFD20
#define UI_COLOR_GRAY       0x8410
#define UI_COLOR_DARK_GRAY  0x4208
#define UI_COLOR_LIGHT_GRAY 0xC618

// VU meter color gradient (green to yellow to red)
#define UI_COLOR_VU_GREEN   0x07E0
#define UI_COLOR_VU_YELLOW  0xFFE0
#define UI_COLOR_VU_ORANGE  0xFD20
#define UI_COLOR_VU_RED     0xF800

#endif // __UI_ASSETS_H