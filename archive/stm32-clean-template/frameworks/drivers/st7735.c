/**
 * @file st7735.c
 * @brief ST7735 LCD Driver Implementation
 * 
 * Generic ST7735 driver adapted from WB32L003 project for use with
 * STM32 Clean Architecture Template. 
 * 
 * Usage:
 * 1. Configure SPI1 and Timer2 in CubeMX/HAL
 * 2. Set display pin definitions in pin_config.h
 * 3. Call ST7735_Init() to initialize display
 * 4. Use drawing functions to render graphics and text
 * 
 * Pin Configuration Required:
 * - DISPLAY_CS_Port/Pin: SPI Chip Select
 * - DISPLAY_DC_Port/Pin: Data/Command select
 * - DISPLAY_RST_Port/Pin: Reset pin
 * - DISPLAY_BL_Port/Pin: Backlight control (PWM)
 * 
 * External Dependencies:
 * - hspi1: SPI handle for communication
 * - htim2: Timer handle for backlight PWM (Channel 2)
 */

#include "st7735.h"
#include "pin_config.h"

// External SPI handle - should be declared in main.c or hal.c
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim2;

// MADCTL bits
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

// Basic 7x10 font definition
static const uint8_t Font7x10_Table[] = {
    // Space (ASCII 32)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // ! (ASCII 33)
    0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x00, 0x20, 0x00, 0x00,
    // " (ASCII 34)
    0x00, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // # (ASCII 35)
    0x00, 0x00, 0x50, 0xF8, 0x50, 0xF8, 0x50, 0x00, 0x00, 0x00,
    // $ (ASCII 36)
    0x00, 0x20, 0x78, 0xA0, 0x70, 0x28, 0xF0, 0x20, 0x00, 0x00,
    // % (ASCII 37)
    0x00, 0x00, 0xC8, 0xC8, 0x10, 0x20, 0x4C, 0x4C, 0x00, 0x00,
    // & (ASCII 38)
    0x00, 0x40, 0xA0, 0xA0, 0x40, 0xA8, 0x90, 0x68, 0x00, 0x00,
    // ' (ASCII 39)
    0x00, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // ( (ASCII 40)
    0x00, 0x10, 0x20, 0x40, 0x40, 0x40, 0x20, 0x10, 0x00, 0x00,
    // ) (ASCII 41)
    0x00, 0x40, 0x20, 0x10, 0x10, 0x10, 0x20, 0x40, 0x00, 0x00,
    // * (ASCII 42)
    0x00, 0x00, 0x20, 0xA8, 0x70, 0xA8, 0x20, 0x00, 0x00, 0x00,
    // + (ASCII 43)
    0x00, 0x00, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x00, 0x00, 0x00,
    // , (ASCII 44)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00,
    // - (ASCII 45)
    0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00,
    // . (ASCII 46)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    // / (ASCII 47)
    0x00, 0x08, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80, 0x00, 0x00,
    // 0 (ASCII 48)
    0x00, 0x00, 0x70, 0x88, 0x98, 0xA8, 0xC8, 0x70, 0x00, 0x00,
    // 1 (ASCII 49)
    0x00, 0x00, 0x20, 0x60, 0x20, 0x20, 0x20, 0x70, 0x00, 0x00,
    // 2 (ASCII 50)
    0x00, 0x00, 0x70, 0x88, 0x08, 0x30, 0x40, 0xF8, 0x00, 0x00,
    // 3 (ASCII 51)
    0x00, 0x00, 0x70, 0x88, 0x30, 0x08, 0x88, 0x70, 0x00, 0x00,
    // 4 (ASCII 52)
    0x00, 0x00, 0x10, 0x30, 0x50, 0x90, 0xF8, 0x10, 0x00, 0x00,
    // 5 (ASCII 53)
    0x00, 0x00, 0xF8, 0x80, 0xF0, 0x08, 0x88, 0x70, 0x00, 0x00,
    // 6 (ASCII 54)
    0x00, 0x00, 0x38, 0x40, 0xF0, 0x88, 0x88, 0x70, 0x00, 0x00,
    // 7 (ASCII 55)
    0x00, 0x00, 0xF8, 0x08, 0x10, 0x20, 0x40, 0x40, 0x00, 0x00,
    // 8 (ASCII 56)
    0x00, 0x00, 0x70, 0x88, 0x70, 0x88, 0x88, 0x70, 0x00, 0x00,
    // 9 (ASCII 57)
    0x00, 0x00, 0x70, 0x88, 0x88, 0x78, 0x08, 0x70, 0x00, 0x00,
    // : (ASCII 58)
    0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    // ; (ASCII 59)
    0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00,
    // < (ASCII 60)
    0x00, 0x00, 0x08, 0x10, 0x20, 0x10, 0x08, 0x00, 0x00, 0x00,
    // = (ASCII 61)
    0x00, 0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00,
    // > (ASCII 62)
    0x00, 0x00, 0x40, 0x20, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00,
    // ? (ASCII 63)
    0x00, 0x00, 0x70, 0x88, 0x10, 0x20, 0x00, 0x20, 0x00, 0x00,
    // @ (ASCII 64)
    0x00, 0x00, 0x70, 0x88, 0xB8, 0xA8, 0xB8, 0x70, 0x00, 0x00,
    // A (ASCII 65)
    0x00, 0x00, 0x20, 0x50, 0x88, 0xF8, 0x88, 0x88, 0x00, 0x00,
    // B (ASCII 66)
    0x00, 0x00, 0xF0, 0x88, 0xF0, 0x88, 0x88, 0xF0, 0x00, 0x00,
    // C (ASCII 67)
    0x00, 0x00, 0x70, 0x88, 0x80, 0x80, 0x88, 0x70, 0x00, 0x00,
    // D (ASCII 68)
    0x00, 0x00, 0xF0, 0x88, 0x88, 0x88, 0x88, 0xF0, 0x00, 0x00,
    // E (ASCII 69)
    0x00, 0x00, 0xF8, 0x80, 0xF0, 0x80, 0x80, 0xF8, 0x00, 0x00,
    // F (ASCII 70)
    0x00, 0x00, 0xF8, 0x80, 0xF0, 0x80, 0x80, 0x80, 0x00, 0x00,
    // G (ASCII 71)
    0x00, 0x00, 0x70, 0x88, 0x80, 0x98, 0x88, 0x70, 0x00, 0x00,
    // H (ASCII 72)
    0x00, 0x00, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88, 0x00, 0x00,
    // I (ASCII 73)
    0x00, 0x00, 0x70, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00, 0x00,
    // J (ASCII 74)
    0x00, 0x00, 0x38, 0x10, 0x10, 0x10, 0x90, 0x60, 0x00, 0x00,
    // K (ASCII 75)
    0x00, 0x00, 0x88, 0x90, 0xA0, 0xC0, 0xA0, 0x90, 0x00, 0x00,
    // L (ASCII 76)
    0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0xF8, 0x00, 0x00,
    // M (ASCII 77)
    0x00, 0x00, 0x88, 0xD8, 0xA8, 0x88, 0x88, 0x88, 0x00, 0x00,
    // N (ASCII 78)
    0x00, 0x00, 0x88, 0xC8, 0xA8, 0x98, 0x88, 0x88, 0x00, 0x00,
    // O (ASCII 79)
    0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00,
    // P (ASCII 80)
    0x00, 0x00, 0xF0, 0x88, 0x88, 0xF0, 0x80, 0x80, 0x00, 0x00,
    // Q (ASCII 81)
    0x00, 0x00, 0x70, 0x88, 0x88, 0xA8, 0x90, 0x68, 0x00, 0x00,
    // R (ASCII 82)
    0x00, 0x00, 0xF0, 0x88, 0x88, 0xF0, 0xA0, 0x90, 0x00, 0x00,
    // S (ASCII 83)
    0x00, 0x00, 0x70, 0x88, 0x60, 0x18, 0x88, 0x70, 0x00, 0x00,
    // T (ASCII 84)
    0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00,
    // U (ASCII 85)
    0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00,
    // V (ASCII 86)
    0x00, 0x00, 0x88, 0x88, 0x88, 0x50, 0x50, 0x20, 0x00, 0x00,
    // W (ASCII 87)
    0x00, 0x00, 0x88, 0x88, 0x88, 0xA8, 0xD8, 0x88, 0x00, 0x00,
    // X (ASCII 88)
    0x00, 0x00, 0x88, 0x50, 0x20, 0x20, 0x50, 0x88, 0x00, 0x00,
    // Y (ASCII 89)
    0x00, 0x00, 0x88, 0x88, 0x50, 0x20, 0x20, 0x20, 0x00, 0x00,
    // Z (ASCII 90)
    0x00, 0x00, 0xF8, 0x08, 0x10, 0x20, 0x40, 0xF8, 0x00, 0x00,
};

// Number of characters in font table
#define FONT_CHAR_COUNT 91  // ASCII 32-90 (space through Z)

// Font definition
FontDef Font_7x10 = {7, 10, (const uint16_t*)Font7x10_Table};

/**
 * @brief Send data via SPI
 * @param data: byte to send
 */
static void ST7735_SpiWrite(uint8_t data)
{
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
}

/**
 * @brief Write command to ST7735
 * @param cmd: command byte
 */
void ST7735_WriteCommand(uint8_t cmd)
{
    ST7735_DC_LOW();
    ST7735_CS_LOW();
    ST7735_SpiWrite(cmd);
    ST7735_CS_HIGH();
}

/**
 * @brief Write data to ST7735
 * @param data: data byte
 */
void ST7735_WriteData(uint8_t data)
{
    ST7735_DC_HIGH();
    ST7735_CS_LOW();
    ST7735_SpiWrite(data);
    ST7735_CS_HIGH();
}

/**
 * @brief Write 16-bit data to ST7735
 * @param data: 16-bit data
 */
void ST7735_WriteData16(uint16_t data)
{
    ST7735_DC_HIGH();
    ST7735_CS_LOW();
    ST7735_SpiWrite(data >> 8);
    ST7735_SpiWrite(data & 0xFF);
    ST7735_CS_HIGH();
}

/**
 * @brief Reset ST7735 display
 */
void ST7735_Reset(void)
{
    ST7735_RST_HIGH();
    HAL_Delay(10);
    ST7735_RST_LOW();
    HAL_Delay(10);
    ST7735_RST_HIGH();
    HAL_Delay(120);
}

/**
 * @brief Initialize ST7735 display
 */
void ST7735_Init(void)
{
    ST7735_Reset();
    
    ST7735_WriteCommand(ST7735_SWRESET);
    HAL_Delay(150);
    
    ST7735_WriteCommand(ST7735_SLPOUT);
    HAL_Delay(500);
    
    ST7735_WriteCommand(ST7735_FRMCTR1);
    ST7735_WriteData(0x01);
    ST7735_WriteData(0x2C);
    ST7735_WriteData(0x2D);
    
    ST7735_WriteCommand(ST7735_FRMCTR2);
    ST7735_WriteData(0x01);
    ST7735_WriteData(0x2C);
    ST7735_WriteData(0x2D);
    
    ST7735_WriteCommand(ST7735_FRMCTR3);
    ST7735_WriteData(0x01);
    ST7735_WriteData(0x2C);
    ST7735_WriteData(0x2D);
    ST7735_WriteData(0x01);
    ST7735_WriteData(0x2C);
    ST7735_WriteData(0x2D);
    
    ST7735_WriteCommand(ST7735_INVCTR);
    ST7735_WriteData(0x07);
    
    ST7735_WriteCommand(ST7735_PWCTR1);
    ST7735_WriteData(0xA2);
    ST7735_WriteData(0x02);
    ST7735_WriteData(0x84);
    
    ST7735_WriteCommand(ST7735_PWCTR2);
    ST7735_WriteData(0xC5);
    
    ST7735_WriteCommand(ST7735_PWCTR3);
    ST7735_WriteData(0x0A);
    ST7735_WriteData(0x00);
    
    ST7735_WriteCommand(ST7735_PWCTR4);
    ST7735_WriteData(0x8A);
    ST7735_WriteData(0x2A);
    
    ST7735_WriteCommand(ST7735_PWCTR5);
    ST7735_WriteData(0x8A);
    ST7735_WriteData(0xEE);
    
    ST7735_WriteCommand(ST7735_VMCTR1);
    ST7735_WriteData(0x0E);
    
    ST7735_WriteCommand(ST7735_INVOFF);
    
    ST7735_WriteCommand(ST7735_MADCTL);
    ST7735_WriteData(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
    
    ST7735_WriteCommand(ST7735_COLMOD);
    ST7735_WriteData(0x05);
    
    ST7735_WriteCommand(ST7735_CASET);
    ST7735_WriteData(0x00);
    ST7735_WriteData(0x00);
    ST7735_WriteData(0x00);
    ST7735_WriteData(0x9F);
    
    ST7735_WriteCommand(ST7735_RASET);
    ST7735_WriteData(0x00);
    ST7735_WriteData(0x00);
    ST7735_WriteData(0x00);
    ST7735_WriteData(0x4F);
    
    ST7735_WriteCommand(ST7735_GMCTRP1);
    ST7735_WriteData(0x02);
    ST7735_WriteData(0x1C);
    ST7735_WriteData(0x07);
    ST7735_WriteData(0x12);
    ST7735_WriteData(0x37);
    ST7735_WriteData(0x32);
    ST7735_WriteData(0x29);
    ST7735_WriteData(0x2D);
    ST7735_WriteData(0x29);
    ST7735_WriteData(0x25);
    ST7735_WriteData(0x2B);
    ST7735_WriteData(0x39);
    ST7735_WriteData(0x00);
    ST7735_WriteData(0x01);
    ST7735_WriteData(0x03);
    ST7735_WriteData(0x10);
    
    ST7735_WriteCommand(ST7735_GMCTRN1);
    ST7735_WriteData(0x03);
    ST7735_WriteData(0x1D);
    ST7735_WriteData(0x07);
    ST7735_WriteData(0x06);
    ST7735_WriteData(0x2E);
    ST7735_WriteData(0x2C);
    ST7735_WriteData(0x29);
    ST7735_WriteData(0x2D);
    ST7735_WriteData(0x2E);
    ST7735_WriteData(0x2E);
    ST7735_WriteData(0x37);
    ST7735_WriteData(0x3F);
    ST7735_WriteData(0x00);
    ST7735_WriteData(0x00);
    ST7735_WriteData(0x02);
    ST7735_WriteData(0x10);
    
    ST7735_WriteCommand(ST7735_NORON);
    HAL_Delay(10);
    
    ST7735_WriteCommand(ST7735_DISPON);
    HAL_Delay(100);
    
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_SetBacklight(2);
}

/**
 * @brief Set address window for display operations
 * @param x0: start x coordinate
 * @param y0: start y coordinate  
 * @param x1: end x coordinate
 * @param y1: end y coordinate
 */
void ST7735_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ST7735_WriteCommand(ST7735_CASET);
    ST7735_WriteData(0x00);
    ST7735_WriteData(x0);
    ST7735_WriteData(0x00);
    ST7735_WriteData(x1);
    
    ST7735_WriteCommand(ST7735_RASET);
    ST7735_WriteData(0x00);
    ST7735_WriteData(y0);
    ST7735_WriteData(0x00);
    ST7735_WriteData(y1);
    
    ST7735_WriteCommand(ST7735_RAMWR);
}

/**
 * @brief Fill rectangle with color
 * @param x: x coordinate
 * @param y: y coordinate
 * @param w: width
 * @param h: height
 * @param color: 16-bit color
 */
void ST7735_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if ((x + w - 1) >= ST7735_WIDTH) w = ST7735_WIDTH - x;
    if ((y + h - 1) >= ST7735_HEIGHT) h = ST7735_HEIGHT - y;
    
    ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    
    ST7735_DC_HIGH();
    ST7735_CS_LOW();
    
    for (uint32_t i = 0; i < w * h; i++)
    {
        ST7735_SpiWrite(color >> 8);
        ST7735_SpiWrite(color & 0xFF);
    }
    
    ST7735_CS_HIGH();
}

/**
 * @brief Fill entire screen with color
 * @param color: 16-bit color
 */
void ST7735_FillScreen(uint16_t color)
{
    ST7735_FillRect(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}

/**
 * @brief Draw single pixel
 * @param x: x coordinate
 * @param y: y coordinate
 * @param color: 16-bit color
 */
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    
    ST7735_SetAddressWindow(x, y, x, y);
    ST7735_WriteData16(color);
}

/**
 * @brief Draw image from data array
 * @param x: x coordinate
 * @param y: y coordinate
 * @param w: width
 * @param h: height
 * @param data: pointer to 16-bit color data
 */
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data)
{
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if ((x + w - 1) >= ST7735_WIDTH) w = ST7735_WIDTH - x;
    if ((y + h - 1) >= ST7735_HEIGHT) h = ST7735_HEIGHT - y;
    
    ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    
    ST7735_DC_HIGH();
    ST7735_CS_LOW();
    
    for (uint32_t i = 0; i < w * h; i++)
    {
        uint16_t pixel = data[i];
        ST7735_SpiWrite(pixel >> 8);
        ST7735_SpiWrite(pixel & 0xFF);
    }
    
    ST7735_CS_HIGH();
}

/**
 * @brief Write single character
 * @param x: x coordinate
 * @param y: y coordinate
 * @param ch: character to write
 * @param font: font definition
 * @param color: text color
 * @param bgcolor: background color
 */
void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
    uint32_t i, j;
    
    // Check if character is printable and in our range
    if (ch < 32 || ch > 90) ch = 32; // Default to space if not supported
    
    // Get character data offset
    uint8_t char_index = ch - 32;
    if (char_index >= FONT_CHAR_COUNT) {
        char_index = 0; // Default to space
    }
    
    ST7735_SetAddressWindow(x, y, x + font.width - 1, y + font.height - 1);
    
    ST7735_DC_HIGH();
    ST7735_CS_LOW();
    
    for (j = 0; j < font.height; j++) {
        uint8_t line = Font7x10_Table[char_index * font.height + j];
        for (i = 0; i < font.width; i++) {
            if (line & (1 << (font.width - 1 - i))) {
                ST7735_SpiWrite(color >> 8);
                ST7735_SpiWrite(color & 0xFF);
            } else {
                ST7735_SpiWrite(bgcolor >> 8);
                ST7735_SpiWrite(bgcolor & 0xFF);
            }
        }
    }
    
    ST7735_CS_HIGH();
}

/**
 * @brief Write string
 * @param x: x coordinate
 * @param y: y coordinate
 * @param str: string to write
 * @param font: font definition
 * @param color: text color
 * @param bgcolor: background color
 */
void ST7735_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor)
{
    uint16_t curr_x = x;
    
    while (*str) {
        if (curr_x + font.width >= ST7735_WIDTH) {
            curr_x = x;
            y += font.height;
            if (y + font.height >= ST7735_HEIGHT) {
                break;
            }
            
            if (*str == ' ') {
                str++;
                continue;
            }
        }
        
        ST7735_WriteChar(curr_x, y, *str, font, color, bgcolor);
        curr_x += font.width;
        str++;
    }
}

/**
 * @brief Set backlight level using PWM
 * @param level: backlight level (0-3)
 */
void ST7735_SetBacklight(uint8_t level)
{
    uint16_t pulse = 0;
    
    switch (level)
    {
        case 0:
            pulse = 0;
            break;
        case 1:
            pulse = 250;
            break;
        case 2:
            pulse = 600;
            break;
        case 3:
            pulse = 999;
            break;
        default:
            pulse = 600;
            break;
    }
    
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);
}