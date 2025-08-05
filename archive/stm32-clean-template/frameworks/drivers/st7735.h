#ifndef __ST7735_H
#define __ST7735_H

#ifdef STM32F0
#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F2)
#include "stm32f2xx_hal.h"
#elif defined(STM32F3)
#include "stm32f3xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#elif defined(STM32F7)
#include "stm32f7xx_hal.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#elif defined(STM32L1)
#include "stm32l1xx_hal.h"
#elif defined(STM32L4)
#include "stm32l4xx_hal.h"
#elif defined(STM32G0)
#include "stm32g0xx_hal.h"
#elif defined(STM32G4)
#include "stm32g4xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#else
#include "stm32f0xx_hal.h"  // Default fallback
#endif

/**
 * @brief ST7735 LCD Driver
 * 
 * Generic ST7735 driver for various STM32 MCUs.
 * Pin assignments are configured through pin_config.h
 */

/* Display dimensions */
#define ST7735_WIDTH  80
#define ST7735_HEIGHT 160

/* ST7735 Commands */
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

/* MADCTL bits */
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

/* Color definitions */
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF

/* Font structure */
typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint16_t *data;
} FontDef;

extern FontDef Font_7x10;

/* Function prototypes */
void ST7735_Init(void);
void ST7735_WriteCommand(uint8_t cmd);
void ST7735_WriteData(uint8_t data);
void ST7735_WriteData16(uint16_t data);
void ST7735_Reset(void);
void ST7735_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ST7735_FillScreen(uint16_t color);
void ST7735_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void ST7735_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
void ST7735_SetBacklight(uint8_t level);

/* GPIO control macros - to be defined in pin_config.h */
#define ST7735_CS_LOW()  HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_RESET)
#define ST7735_CS_HIGH() HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_SET)
#define ST7735_DC_LOW()  HAL_GPIO_WritePin(DISPLAY_DC_Port, DISPLAY_DC_Pin, GPIO_PIN_RESET)
#define ST7735_DC_HIGH() HAL_GPIO_WritePin(DISPLAY_DC_Port, DISPLAY_DC_Pin, GPIO_PIN_SET)
#define ST7735_RST_LOW() HAL_GPIO_WritePin(DISPLAY_RST_Port, DISPLAY_RST_Pin, GPIO_PIN_RESET)
#define ST7735_RST_HIGH() HAL_GPIO_WritePin(DISPLAY_RST_Port, DISPLAY_RST_Pin, GPIO_PIN_SET)

#endif /* __ST7735_H */