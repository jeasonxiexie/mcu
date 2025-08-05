#ifndef __UI_SIMPLE_H
#define __UI_SIMPLE_H

#include "stdint.h"
#include "stdbool.h"

// 显示屏参数
#define DISPLAY_WIDTH  160
#define DISPLAY_HEIGHT 80

// 颜色定义 (RGB565)
#define COLOR_BLACK    0x0000
#define COLOR_WHITE    0xFFFF
#define COLOR_RED      0xF800
#define COLOR_GREEN    0x07E0
#define COLOR_BLUE     0x001F
#define COLOR_GRAY     0x8410
#define COLOR_DARKGRAY 0x4208

// 简单的UI函数
void ui_draw_boot_screen(void);
void ui_draw_power_off_screen(void);
void ui_draw_volume_screen(uint8_t volume);
void ui_draw_filled_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ui_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ui_draw_text(uint16_t x, uint16_t y, const char* text, uint16_t color);
void ui_clear_screen(uint16_t color);

#endif // __UI_SIMPLE_H