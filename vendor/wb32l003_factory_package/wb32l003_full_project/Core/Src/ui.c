#include "ui.h"
#include "ui_assets.h"
#include "st7735.h"

static ui_mode_t current_mode = UI_MODE_BOOT;
static audio_mode_t audio_mode = AUDIO_MODE_STEREO;
static uint8_t current_frame = 0;
static uint32_t last_frame_time = 0;
static uint8_t vu_left_level = 0;
static uint8_t vu_right_level = 0;
static battery_level_t last_battery_level = BATTERY_LEVEL_4;

// Placeholder for boot logo (160x80 RGB565)
const uint16_t boot_logo[160*80] = {
    0x0000  // Black screen placeholder - replace with actual boot logo
};

// Placeholder for shutdown logo (160x80 RGB565) 
const uint16_t off_logo[160*80] = {
    0x0000  // Black screen placeholder - replace with actual shutdown logo
};

// VU Meter pointer positions for different levels (0-31)
static const struct {
    uint8_t x;
    uint8_t y;
    int8_t angle;  // rotation angle in degrees
} pointer_positions[32] = {
    {20, 60, -45}, {22, 58, -42}, {24, 56, -39}, {26, 54, -36},
    {28, 52, -33}, {30, 50, -30}, {32, 48, -27}, {34, 46, -24},
    {36, 44, -21}, {38, 42, -18}, {40, 40, -15}, {42, 38, -12},
    {44, 36, -9},  {46, 34, -6},  {48, 32, -3},  {50, 30, 0},
    {52, 32, 3},   {54, 34, 6},   {56, 36, 9},   {58, 38, 12},
    {60, 40, 15},  {62, 42, 18},  {64, 44, 21},  {66, 46, 24},
    {68, 48, 27},  {70, 50, 30},  {72, 52, 33},  {74, 54, 36},
    {76, 56, 39},  {78, 58, 42},  {80, 60, 45},  {82, 62, 48}
};

void UI_Init(void)
{
    current_mode = UI_MODE_BOOT;
    current_frame = 0;
    last_frame_time = HAL_GetTick();
}

void UI_ShowBootLogo(void)
{
    current_mode = UI_MODE_BOOT;
    // Display is 80x160 in portrait mode
    ST7735_DrawImage(0, 40, 80, 80, boot_logo);
}

void UI_ShowOffLogo(void)
{
    current_mode = UI_MODE_SHUTDOWN;
    // Display is 80x160 in portrait mode
    ST7735_DrawImage(0, 40, 80, 80, off_logo);
}

void UI_DrawBackground(void)
{
    // Due to flash size constraints, use simple drawing instead of full background image
    ST7735_FillScreen(ST7735_BLACK);
    
    // Draw VU meter scale
    uint16_t scale_color = UI_COLOR_DARK_GRAY;
    for (int i = 0; i <= 8; i++)
    {
        uint8_t x = 10 + i * 7;
        ST7735_DrawPixel(x, 100, scale_color);
        ST7735_DrawPixel(x, 101, scale_color);
    }
    
    // Draw VU meter arc guides
    for (int i = 0; i < 20; i++)
    {
        uint8_t x = 20 + i * 2;
        uint8_t y = 95 - (i * i) / 20;
        ST7735_DrawPixel(x, y, scale_color);
    }
}

void UI_SetAudioMode(audio_mode_t mode)
{
    audio_mode = mode;
    
    // Clear the mode text area first (adjusted for 80x160 display)
    ST7735_FillRect(10, 145, 60, 12, ST7735_BLACK);
    
    // Draw MONO or STEREO text at bottom
    if (mode == AUDIO_MODE_MONO)
    {
        // Draw MONO text using simple rectangles due to flash constraints
        // Position adjusted for 80x160 portrait display
        ST7735_FillRect(20, 146, 40, 10, UI_COLOR_ORANGE);
        // Draw "MONO" text placeholder
        ST7735_FillRect(22, 148, 36, 6, ST7735_BLACK);
        ST7735_FillRect(24, 148, 3, 6, UI_COLOR_ORANGE); // M
        ST7735_FillRect(29, 148, 3, 6, UI_COLOR_ORANGE); // O
        ST7735_FillRect(34, 148, 3, 6, UI_COLOR_ORANGE); // N
        ST7735_FillRect(39, 148, 3, 6, UI_COLOR_ORANGE); // O
    }
    else
    {
        // Draw STEREO text using simple rectangles due to flash constraints
        // Position adjusted for 80x160 portrait display
        ST7735_FillRect(15, 146, 50, 10, UI_COLOR_CYAN);
        // Draw "STEREO" text placeholder
        ST7735_FillRect(17, 148, 46, 6, ST7735_BLACK);
        ST7735_FillRect(19, 148, 3, 6, UI_COLOR_CYAN); // S
        ST7735_FillRect(24, 148, 3, 6, UI_COLOR_CYAN); // T
        ST7735_FillRect(29, 148, 3, 6, UI_COLOR_CYAN); // E
        ST7735_FillRect(34, 148, 3, 6, UI_COLOR_CYAN); // R
        ST7735_FillRect(39, 148, 3, 6, UI_COLOR_CYAN); // E
        ST7735_FillRect(44, 148, 3, 6, UI_COLOR_CYAN); // O
    }
}

void UI_UpdateVUMeter(uint8_t left_level, uint8_t right_level)
{
    // Clamp levels to 0-31 range
    if (left_level > 31) left_level = 31;
    if (right_level > 31) right_level = 31;
    
    vu_left_level = left_level;
    vu_right_level = right_level;
    
    if (current_mode != UI_MODE_VU_METER)
    {
        current_mode = UI_MODE_VU_METER;
        UI_DrawBackground();
        UI_SetAudioMode(audio_mode);
    }
    
    // Display based on audio mode
    if (audio_mode == AUDIO_MODE_STEREO)
    {
        // STEREO mode - display separate left and right channels
        
        // Left channel
        uint8_t left_x = 10;
        uint8_t bar_y = 60;
        
        uint16_t left_color = UI_COLOR_VU_GREEN;
        if (left_level > 24) left_color = UI_COLOR_VU_RED;
        else if (left_level > 16) left_color = UI_COLOR_VU_ORANGE;
        else if (left_level > 8) left_color = UI_COLOR_VU_YELLOW;
        
        ST7735_FillRect(left_x, bar_y - 40, 8, 40, ST7735_BLACK);
        uint8_t left_height = (left_level * 40) / 32;
        ST7735_FillRect(left_x, bar_y - left_height, 8, left_height, left_color);
        
        // Right channel
        uint8_t right_x = 65;
        
        uint16_t right_color = UI_COLOR_VU_GREEN;
        if (right_level > 24) right_color = UI_COLOR_VU_RED;
        else if (right_level > 16) right_color = UI_COLOR_VU_ORANGE;
        else if (right_level > 8) right_color = UI_COLOR_VU_YELLOW;
        
        ST7735_FillRect(right_x, bar_y - 40, 8, 40, ST7735_BLACK);
        uint8_t right_height = (right_level * 40) / 32;
        ST7735_FillRect(right_x, bar_y - right_height, 8, right_height, right_color);
        
        // Channel indicators
        ST7735_FillRect(5, 130, 10, 8, left_level > 5 ? UI_COLOR_VU_GREEN : ST7735_BLACK);
        ST7735_FillRect(65, 130, 10, 8, right_level > 5 ? UI_COLOR_VU_GREEN : ST7735_BLACK);
        
        // Draw "L" and "R" labels
        ST7735_FillRect(7, 120, 6, 7, UI_COLOR_WHITE);  // L
        ST7735_FillRect(67, 120, 6, 7, UI_COLOR_WHITE); // R
    }
    else  // MONO mode
    {
        // MONO mode - display single centered bar
        
        // Use the average of left and right (already done in main.c)
        uint8_t mono_level = (left_level + right_level) / 2;
        
        // Center bar position
        uint8_t center_x = 36;  // Center of 80px display
        uint8_t bar_y = 60;
        uint8_t bar_width = 16; // Wider bar for mono
        
        uint16_t color = UI_COLOR_VU_GREEN;
        if (mono_level > 24) color = UI_COLOR_VU_RED;
        else if (mono_level > 16) color = UI_COLOR_VU_ORANGE;
        else if (mono_level > 8) color = UI_COLOR_VU_YELLOW;
        
        // Clear both sides (in case switching from stereo)
        ST7735_FillRect(10, bar_y - 40, 8, 40, ST7735_BLACK);
        ST7735_FillRect(65, bar_y - 40, 8, 40, ST7735_BLACK);
        
        // Draw center mono bar
        ST7735_FillRect(center_x, bar_y - 40, bar_width, 40, ST7735_BLACK);
        uint8_t bar_height = (mono_level * 40) / 32;
        ST7735_FillRect(center_x, bar_y - bar_height, bar_width, bar_height, color);
        
        // Clear L/R indicators
        ST7735_FillRect(5, 120, 10, 18, ST7735_BLACK);
        ST7735_FillRect(65, 120, 10, 18, ST7735_BLACK);
        
        // Draw "MONO" indicator in center
        ST7735_FillRect(30, 130, 20, 8, mono_level > 5 ? UI_COLOR_ORANGE : ST7735_BLACK);
    }
    
    // Draw pointer indicators for visual reference
    // Note: The actual pointer image would be better, but for now we use the volume blocks
    // which already show the level visually
}

void UI_UpdateAnimation(void)
{
    uint32_t current_time = HAL_GetTick();
    
    if (current_time - last_frame_time >= METER_FRAME_DELAY)
    {
        last_frame_time = current_time;
        
        if (current_mode == UI_MODE_VU_METER)
        {
            // Animate the volume display bars
            // This would cycle through volume_blocks based on audio levels
            
            // For now, just update with current VU levels
            UI_UpdateVUMeter(vu_left_level, vu_right_level);
        }
    }
}

void UI_UpdateBatteryIcon(battery_level_t level)
{
    // Position adjusted for 80x160 portrait display
    uint8_t x = 50;
    uint8_t y = 5;
    
    last_battery_level = level;
    
    // Clear battery area
    ST7735_FillRect(x, y, BATTERY_ICON_WIDTH + 2, BATTERY_ICON_HEIGHT + 2, ST7735_BLACK);
    
    // Draw battery outline manually due to flash constraints
    // Battery body
    for (uint8_t i = 0; i < BATTERY_ICON_WIDTH; i++)
    {
        ST7735_DrawPixel(x + i, y, ST7735_WHITE);
        ST7735_DrawPixel(x + i, y + BATTERY_ICON_HEIGHT - 1, ST7735_WHITE);
    }
    for (uint8_t i = 0; i < BATTERY_ICON_HEIGHT; i++)
    {
        ST7735_DrawPixel(x, y + i, ST7735_WHITE);
        ST7735_DrawPixel(x + BATTERY_ICON_WIDTH - 1, y + i, ST7735_WHITE);
    }
    // Battery terminal
    ST7735_FillRect(x + BATTERY_ICON_WIDTH, y + 2, 2, 6, ST7735_WHITE);
    
    // Draw battery fill bars based on level
    uint8_t bar_count = 0;
    uint16_t bar_color = UI_COLOR_VU_GREEN;
    
    switch (level)
    {
        case BATTERY_LEVEL_4:
            bar_count = 4;
            bar_color = UI_COLOR_VU_GREEN;
            break;
        case BATTERY_LEVEL_3:
            bar_count = 3;
            bar_color = UI_COLOR_VU_GREEN;
            break;
        case BATTERY_LEVEL_2:
            bar_count = 2;
            bar_color = UI_COLOR_VU_YELLOW;
            break;
        case BATTERY_LEVEL_1:
            bar_count = 1;
            bar_color = UI_COLOR_VU_ORANGE;
            break;
        case BATTERY_LEVEL_0:
            bar_count = 0;
            // Will flash red in the animation
            break;
    }
    
    // Draw battery fill bars
    if (bar_count > 0)
    {
        // Draw green bars inside the battery icon
        // Each bar represents 25% charge
        for (uint8_t i = 0; i < bar_count; i++)
        {
            uint8_t bar_x = x + 2 + (i * 4);
            uint8_t bar_y = y + 2;
            
            // Use the battery bar image or draw colored rectangle
            if (level == BATTERY_LEVEL_0)
            {
                // Flash red for critical battery
                if ((HAL_GetTick() / 500) % 2)
                {
                    ST7735_FillRect(bar_x, bar_y, 3, BATTERY_BAR_HEIGHT, UI_COLOR_VU_RED);
                }
            }
            else
            {
                // Normal battery level - draw bar with appropriate color
                ST7735_FillRect(bar_x, bar_y, 3, BATTERY_BAR_HEIGHT, bar_color);
            }
        }
    }
    else if (level == BATTERY_LEVEL_0)
    {
        // Flash entire battery area red when critical
        if ((HAL_GetTick() / 500) % 2)
        {
            ST7735_FillRect(x + 2, y + 2, BATTERY_ICON_WIDTH - 4, BATTERY_BAR_HEIGHT, UI_COLOR_VU_RED);
        }
    }
}

void UI_Clear(void)
{
    ST7735_FillScreen(ST7735_BLACK);
}