#include "st7735.h"
#include "pin_config.h"

/* Software SPI implementation for LCD interface */
/* o3 verified: Using PB5 (SCL) and PB3 (SDA) for bit-banged SPI */

/* Macros for software SPI control */
#define LCD_SCL_HIGH()  HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_SET)
#define LCD_SCL_LOW()   HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_RESET)
#define LCD_SDA_HIGH()  HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_SET)
#define LCD_SDA_LOW()   HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_RESET)

static void ST7735_SpiWrite(uint8_t data)
{
    uint8_t bit;
    
    /* Send 8 bits, MSB first */
    for (bit = 0x80; bit != 0; bit >>= 1)
    {
        /* Set data line */
        if (data & bit) {
            LCD_SDA_HIGH();
        } else {
            LCD_SDA_LOW();
        }
        
        /* Clock pulse */
        LCD_SCL_LOW();
        __asm__("nop");  /* Small delay */
        LCD_SCL_HIGH();
        __asm__("nop");
    }
}

void ST7735_WriteCommand(uint8_t cmd)
{
    ST7735_DC_LOW();
    ST7735_CS_LOW();
    ST7735_SpiWrite(cmd);
    ST7735_CS_HIGH();
}

void ST7735_WriteData(uint8_t data)
{
    ST7735_DC_HIGH();
    ST7735_CS_LOW();
    ST7735_SpiWrite(data);
    ST7735_CS_HIGH();
}

void ST7735_WriteData16(uint16_t data)
{
    ST7735_DC_HIGH();
    ST7735_CS_LOW();
    ST7735_SpiWrite(data >> 8);
    ST7735_SpiWrite(data & 0xFF);
    ST7735_CS_HIGH();
}

void ST7735_Reset(void)
{
    ST7735_RST_HIGH();
    HAL_Delay(10);
    ST7735_RST_LOW();
    HAL_Delay(15);  // Ensure ≥10ms reset pulse as per ChatGPT recommendation
    ST7735_RST_HIGH();
    HAL_Delay(120);
}

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

void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
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

void ST7735_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
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

void ST7735_FillScreen(uint16_t color)
{
    ST7735_FillRect(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}

void ST7735_DrawPixel(uint8_t x, uint8_t y, uint16_t color)
{
    if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    
    ST7735_SetAddressWindow(x, y, x, y);
    ST7735_WriteData16(color);
}

void ST7735_DrawImage(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint16_t *data)
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

void ST7735_SetBacklight(uint8_t level)
{
    /* o3 verified: Two-level backlight control via Q7 transistor */
    /* CLD_BL1 (PB1) controls Q7 PNP transistor - LOW = ON */
    /* CLD_BL2 (PB2) directly drives backlight cathode */
    
    switch (level)
    {
        case 0:  /* Off */
            HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_SET);    /* BL1 OFF */
            HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_SET);    /* BL2 OFF */
            break;
            
        case 1:  /* Low brightness - BL2 only */
            HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_SET);    /* BL1 OFF */
            HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_RESET);  /* BL2 ON */
            break;
            
        case 2:  /* High brightness - BL1 via Q7 */
        case 3:
        default:
            HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_RESET);  /* BL1 ON (Q7 conducts) */
            HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_SET);    /* BL2 OFF */
            break;
    }
}