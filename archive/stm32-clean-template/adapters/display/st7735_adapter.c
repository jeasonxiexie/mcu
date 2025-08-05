#include "st7735_adapter.h"
#include "../../frameworks/drivers/st7735.h"
#include "../../app/pin_config.h"
#include <string.h>

/* Private variables */
static bool display_initialized = false;
static display_mode_t current_mode = DISPLAY_MODE_NORMAL;

/* Private function declarations */
static void st7735_adapter_show_logo(const image_t* logo);

/* Interface implementation */
static bool st7735_init(void)
{
    if (display_initialized) {
        return true;
    }
    
    // Initialize GPIO pins
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    // CS Pin
    GPIO_InitStruct.Pin = DISPLAY_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DISPLAY_CS_Port, &GPIO_InitStruct);
    
    // DC Pin
    GPIO_InitStruct.Pin = DISPLAY_DC_Pin;
    HAL_GPIO_Init(DISPLAY_DC_Port, &GPIO_InitStruct);
    
    // RST Pin
    GPIO_InitStruct.Pin = DISPLAY_RST_Pin;
    HAL_GPIO_Init(DISPLAY_RST_Port, &GPIO_InitStruct);
    
    // Backlight Pin (PWM)
    GPIO_InitStruct.Pin = DISPLAY_BL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
    HAL_GPIO_Init(DISPLAY_BL_Port, &GPIO_InitStruct);
    
    // Initialize ST7735 driver
    ST7735_Init();
    
    display_initialized = true;
    current_mode = DISPLAY_MODE_NORMAL;
    
    return true;
}

static void st7735_clear(uint16_t color)
{
    if (!display_initialized) {
        return;
    }
    
    ST7735_FillScreen(color);
}

static void st7735_draw_image(uint16_t x, uint16_t y, const image_t* image)
{
    if (!display_initialized || image == NULL) {
        return;
    }
    
    // For RGB565 images
    if (image->format == IMAGE_FORMAT_RGB565) {
        ST7735_DrawImage(x, y, image->width, image->height, image->data);
    }
}

static void st7735_draw_rect(const rect_t* rect, uint16_t color)
{
    if (!display_initialized || rect == NULL) {
        return;
    }
    
    ST7735_FillRect(rect->x, rect->y, rect->width, rect->height, color);
}

static void st7735_draw_text(uint16_t x, uint16_t y, const char* text, uint16_t color, uint16_t bg_color)
{
    if (!display_initialized || text == NULL) {
        return;
    }
    
    ST7735_WriteString(x, y, text, Font_7x10, color, bg_color);
}

static void st7735_set_backlight(uint8_t brightness)
{
    if (!display_initialized) {
        return;
    }
    
    // Set PWM duty cycle for backlight
    uint32_t pulse = (brightness * 1000) / 100; // Convert to PWM range
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);
}

static void st7735_set_power_mode(display_mode_t mode)
{
    if (!display_initialized) {
        return;
    }
    
    current_mode = mode;
    
    switch (mode) {
        case DISPLAY_MODE_NORMAL:
            ST7735_WriteCommand(ST7735_SLPOUT);
            HAL_Delay(120);
            st7735_set_backlight(100);
            break;
            
        case DISPLAY_MODE_SLEEP:
            st7735_set_backlight(0);
            ST7735_WriteCommand(ST7735_SLPIN);
            break;
            
        case DISPLAY_MODE_OFF:
            st7735_set_backlight(0);
            ST7735_WriteCommand(ST7735_DISPOFF);
            break;
    }
}

static display_mode_t st7735_get_power_mode(void)
{
    return current_mode;
}

static void st7735_show_startup_logo(const image_t* logo)
{
    st7735_adapter_show_logo(logo);
}

static void st7735_show_shutdown_logo(const image_t* logo)
{
    st7735_adapter_show_logo(logo);
}

/* Private function implementations */
static void st7735_adapter_show_logo(const image_t* logo)
{
    if (!display_initialized || logo == NULL) {
        return;
    }
    
    // Clear screen
    st7735_clear(0x0000); // Black background
    
    // Center the logo
    uint16_t x = (ST7735_WIDTH - logo->width) / 2;
    uint16_t y = (ST7735_HEIGHT - logo->height) / 2;
    
    // Draw logo
    st7735_draw_image(x, y, logo);
}

/* Interface structure */
const display_interface_t st7735_display_interface = {
    .init = st7735_init,
    .clear = st7735_clear,
    .draw_image = st7735_draw_image,
    .draw_rect = st7735_draw_rect,
    .draw_text = st7735_draw_text,
    .set_backlight = st7735_set_backlight,
    .set_power_mode = st7735_set_power_mode,
    .get_power_mode = st7735_get_power_mode,
    .show_startup_logo = st7735_show_startup_logo,
    .show_shutdown_logo = st7735_show_shutdown_logo
};

/* Extern timer handle (to be provided by application) */
extern TIM_HandleTypeDef htim2;