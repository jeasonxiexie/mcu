#ifndef __DISPLAY_INTERFACE_H
#define __DISPLAY_INTERFACE_H

#include "../domain/types.h"

/**
 * @brief Display Hardware Abstraction Interface
 * 
 * This interface abstracts the display hardware from the business logic.
 * Different display drivers (ST7735, ILI9341, etc.) can implement this interface.
 */
typedef struct {
    /**
     * @brief Initialize the display
     * @return true if successful, false otherwise
     */
    bool (*init)(void);
    
    /**
     * @brief Clear the entire display with a color
     * @param color 16-bit RGB565 color
     */
    void (*clear)(uint16_t color);
    
    /**
     * @brief Draw an image at specified position
     * @param x X coordinate
     * @param y Y coordinate
     * @param image Pointer to image data
     */
    void (*draw_image)(uint16_t x, uint16_t y, const image_t* image);
    
    /**
     * @brief Draw a filled rectangle
     * @param rect Rectangle coordinates and size
     * @param color 16-bit RGB565 color
     */
    void (*draw_rect)(const rect_t* rect, uint16_t color);
    
    /**
     * @brief Draw a single pixel
     * @param x X coordinate
     * @param y Y coordinate
     * @param color 16-bit RGB565 color
     */
    void (*draw_pixel)(uint16_t x, uint16_t y, uint16_t color);
    
    /**
     * @brief Set display backlight level
     * @param level Brightness level (0-100)
     */
    void (*set_backlight)(uint8_t level);
    
    /**
     * @brief Set display power mode
     * @param mode Display mode (normal, sleep, off)
     */
    void (*set_power_mode)(display_mode_t mode);
    
    /**
     * @brief Update display buffer (for buffered displays)
     */
    void (*update)(void);
    
    /**
     * @brief Get display dimensions
     * @param width Pointer to store width
     * @param height Pointer to store height
     */
    void (*get_dimensions)(uint16_t* width, uint16_t* height);
} display_interface_t;

/* Color definitions in RGB565 format */
#define COLOR_BLACK      0x0000
#define COLOR_WHITE      0xFFFF
#define COLOR_RED        0xF800
#define COLOR_GREEN      0x07E0
#define COLOR_BLUE       0x001F
#define COLOR_YELLOW     0xFFE0
#define COLOR_CYAN       0x07FF
#define COLOR_MAGENTA    0xF81F
#define COLOR_ORANGE     0xFD20
#define COLOR_GRAY       0x8410
#define COLOR_DARK_GRAY  0x4208
#define COLOR_LIGHT_GRAY 0xC618

/* VU meter colors */
#define COLOR_VU_GREEN   0x07E0
#define COLOR_VU_YELLOW  0xFFE0
#define COLOR_VU_ORANGE  0xFD20
#define COLOR_VU_RED     0xF800

#endif /* __DISPLAY_INTERFACE_H */