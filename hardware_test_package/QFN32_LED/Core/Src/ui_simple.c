#include "ui_simple.h"
#include "st7735.h"

// 简单的5x7字体数据（只包含必要的字符）
static const uint8_t font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // Space
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x44, 0x48, 0x50, 0x60, 0x44}, // Y
};

// 字符到字体索引的映射
static uint8_t char_to_font_index(char c) {
    if (c == ' ') return 0;
    if (c >= '0' && c <= '9') return c - '0' + 1;
    if (c >= 'A' && c <= 'Z') return c - 'A' + 11;
    if (c >= 'a' && c <= 'z') return c - 'a' + 11; // 小写转大写
    return 0; // 默认返回空格
}

void ui_clear_screen(uint16_t color) {
    ST7735_FillScreen(color);
}

void ui_draw_filled_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    ST7735_FillRect(x, y, w, h, color);
}

void ui_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // 画四条边，使用基本的画点功能
    // 上边
    for (uint16_t i = 0; i < w; i++) {
        ST7735_DrawPixel(x + i, y, color);
    }
    // 下边
    for (uint16_t i = 0; i < w; i++) {
        ST7735_DrawPixel(x + i, y + h - 1, color);
    }
    // 左边
    for (uint16_t i = 0; i < h; i++) {
        ST7735_DrawPixel(x, y + i, color);
    }
    // 右边
    for (uint16_t i = 0; i < h; i++) {
        ST7735_DrawPixel(x + w - 1, y + i, color);
    }
}

void ui_draw_text(uint16_t x, uint16_t y, const char* text, uint16_t color) {
    uint16_t cur_x = x;
    while (*text) {
        uint8_t idx = char_to_font_index(*text);
        const uint8_t* char_data = font5x7[idx];
        
        // 绘制字符
        for (int col = 0; col < 5; col++) {
            uint8_t column = char_data[col];
            for (int row = 0; row < 7; row++) {
                if (column & (1 << row)) {
                    ST7735_DrawPixel(cur_x + col, y + row, color);
                }
            }
        }
        
        cur_x += 6; // 字符宽度 + 间距
        text++;
    }
}

void ui_draw_boot_screen(void) {
    // 清屏为黑色
    ui_clear_screen(COLOR_BLACK);
    
    // 画一个简单的边框
    ui_draw_rect(10, 10, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 20, COLOR_WHITE);
    
    // 显示产品名称
    ui_draw_text(45, 25, "HEADPHONE", COLOR_WHITE);
    ui_draw_text(50, 35, "AMPLIFIER", COLOR_WHITE);
    
    // 画一个简单的耳机图标（用几何图形组成）
    // 耳机弧形
    ui_draw_rect(70, 50, 20, 15, COLOR_WHITE);
    // 左耳罩
    ui_draw_filled_rect(65, 60, 8, 10, COLOR_WHITE);
    // 右耳罩
    ui_draw_filled_rect(87, 60, 8, 10, COLOR_WHITE);
}

void ui_draw_power_off_screen(void) {
    // 清屏为黑色
    ui_clear_screen(COLOR_BLACK);
    
    // 画一个简单的电源符号
    // 外圆
    ui_draw_rect(70, 25, 20, 20, COLOR_RED);
    // 中间竖线
    ui_draw_filled_rect(79, 20, 2, 15, COLOR_RED);
    
    // 显示关机文字
    ui_draw_text(50, 55, "POWER OFF", COLOR_RED);
}

void ui_draw_volume_screen(uint8_t volume) {
    // 清屏为黑色
    ui_clear_screen(COLOR_BLACK);
    
    // 显示音量标签
    ui_draw_text(60, 20, "VOL", COLOR_WHITE);
    
    // 显示音量数值（简单实现，最大支持32）
    uint16_t x = 90;
    if (volume >= 30) {
        ui_draw_text(x, 20, "30", COLOR_WHITE);
    } else if (volume >= 20) {
        ui_draw_text(x, 20, "2", COLOR_WHITE);
        x += 6;
        if (volume == 20) ui_draw_text(x, 20, "0", COLOR_WHITE);
        else if (volume == 21) ui_draw_text(x, 20, "1", COLOR_WHITE);
        else if (volume == 22) ui_draw_text(x, 20, "2", COLOR_WHITE);
        else if (volume == 23) ui_draw_text(x, 20, "3", COLOR_WHITE);
        else if (volume == 24) ui_draw_text(x, 20, "4", COLOR_WHITE);
        else if (volume == 25) ui_draw_text(x, 20, "5", COLOR_WHITE);
        else if (volume == 26) ui_draw_text(x, 20, "6", COLOR_WHITE);
        else if (volume == 27) ui_draw_text(x, 20, "7", COLOR_WHITE);
        else if (volume == 28) ui_draw_text(x, 20, "8", COLOR_WHITE);
        else if (volume == 29) ui_draw_text(x, 20, "9", COLOR_WHITE);
    } else if (volume >= 10) {
        ui_draw_text(x, 20, "1", COLOR_WHITE);
        x += 6;
        if (volume == 10) ui_draw_text(x, 20, "0", COLOR_WHITE);
        else if (volume == 11) ui_draw_text(x, 20, "1", COLOR_WHITE);
        else if (volume == 12) ui_draw_text(x, 20, "2", COLOR_WHITE);
        else if (volume == 13) ui_draw_text(x, 20, "3", COLOR_WHITE);
        else if (volume == 14) ui_draw_text(x, 20, "4", COLOR_WHITE);
        else if (volume == 15) ui_draw_text(x, 20, "5", COLOR_WHITE);
        else if (volume == 16) ui_draw_text(x, 20, "6", COLOR_WHITE);
        else if (volume == 17) ui_draw_text(x, 20, "7", COLOR_WHITE);
        else if (volume == 18) ui_draw_text(x, 20, "8", COLOR_WHITE);
        else if (volume == 19) ui_draw_text(x, 20, "9", COLOR_WHITE);
    } else {
        if (volume == 0) ui_draw_text(x, 20, "0", COLOR_WHITE);
        else if (volume == 1) ui_draw_text(x, 20, "1", COLOR_WHITE);
        else if (volume == 2) ui_draw_text(x, 20, "2", COLOR_WHITE);
        else if (volume == 3) ui_draw_text(x, 20, "3", COLOR_WHITE);
        else if (volume == 4) ui_draw_text(x, 20, "4", COLOR_WHITE);
        else if (volume == 5) ui_draw_text(x, 20, "5", COLOR_WHITE);
        else if (volume == 6) ui_draw_text(x, 20, "6", COLOR_WHITE);
        else if (volume == 7) ui_draw_text(x, 20, "7", COLOR_WHITE);
        else if (volume == 8) ui_draw_text(x, 20, "8", COLOR_WHITE);
        else if (volume == 9) ui_draw_text(x, 20, "9", COLOR_WHITE);
    }
    
    // 画音量条背景
    ui_draw_rect(20, 40, 120, 20, COLOR_GRAY);
    
    // 计算音量条长度 (0-32 映射到 0-116)
    uint16_t bar_width = (volume * 116) / 32;
    
    // 画音量条
    if (bar_width > 0) {
        ui_draw_filled_rect(22, 42, bar_width, 16, COLOR_GREEN);
    }
    
    // 画刻度线
    for (int i = 1; i < 8; i++) {
        uint16_t x = 20 + (i * 120 / 8);
        ui_draw_filled_rect(x, 45, 1, 10, COLOR_DARKGRAY);
    }
}