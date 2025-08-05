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
    
    /* Sleep out first - matches demo code */
    ST7735_WriteCommand(0x11);  /* SLPOUT */
    HAL_Delay(120);
    
    /* Frame rate control - matches demo code values */
    ST7735_WriteCommand(ST7735_FRMCTR1);  /* 0xB1 */
    ST7735_WriteData(0x05);
    ST7735_WriteData(0x3C);
    ST7735_WriteData(0x3C);
    
    ST7735_WriteCommand(ST7735_FRMCTR2);  /* 0xB2 */
    ST7735_WriteData(0x05);
    ST7735_WriteData(0x3C);
    ST7735_WriteData(0x3C);
    
    ST7735_WriteCommand(ST7735_FRMCTR3);  /* 0xB3 */
    ST7735_WriteData(0x05);
    ST7735_WriteData(0x3C);
    ST7735_WriteData(0x3C);
    ST7735_WriteData(0x05);
    ST7735_WriteData(0x3C);
    ST7735_WriteData(0x3C);
    
    ST7735_WriteCommand(ST7735_INVCTR);  /* 0xB4 - Dot inversion */
    ST7735_WriteData(0x03);
    
    ST7735_WriteCommand(ST7735_PWCTR1);  /* 0xC0 */
    ST7735_WriteData(0x0E);
    ST7735_WriteData(0x0E);
    ST7735_WriteData(0x04);
    
    ST7735_WriteCommand(ST7735_PWCTR2);  /* 0xC1 */
    ST7735_WriteData(0xC0);
    
    ST7735_WriteCommand(ST7735_PWCTR3);  /* 0xC2 */
    ST7735_WriteData(0x0D);
    ST7735_WriteData(0x00);
    
    ST7735_WriteCommand(ST7735_PWCTR4);  /* 0xC3 */
    ST7735_WriteData(0x8D);
    ST7735_WriteData(0x2A);
    
    ST7735_WriteCommand(ST7735_PWCTR5);  /* 0xC4 */
    ST7735_WriteData(0x8D);
    ST7735_WriteData(0xEE);
    
    ST7735_WriteCommand(ST7735_VMCTR1);  /* 0xC5 */
    ST7735_WriteData(0x09);
    
    ST7735_WriteCommand(ST7735_INVOFF);
    
    ST7735_WriteCommand(ST7735_MADCTL);  /* 0x36 */
    ST7735_WriteData(0xC8);  /* MX=1, MY=1, MV=0, ML=0, RGB=1 */
    
    ST7735_WriteCommand(ST7735_COLMOD);  /* 0x3A */
    ST7735_WriteData(0x05);  /* 16-bit color */
    
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
    /* Hardware team verified backlight control:
     * PB1 (CLD_BL1): LOW = enable Q7 to supply backlight power
     * PB2 (CLD_BL2): HIGH = bright, LOW = dim, High-Z = off
     */
    
    switch (level)
    {
        case 0:  /* Off - PB2 set to high impedance */
        {
            GPIO_InitTypeDef GPIO_InitStruct = {0};
            GPIO_InitStruct.Pin = CLD_BL2_PIN;
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  // High-Z
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            HAL_GPIO_Init(CLD_BL2_PORT, &GPIO_InitStruct);
            
            /* Also disable backlight power */
            HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_SET);    /* BL1 HIGH = OFF */
            break;
        }
            
        case 1:  /* Low brightness */
            /* First ensure PB2 is output mode */
            {
                GPIO_InitTypeDef GPIO_InitStruct = {0};
                GPIO_InitStruct.Pin = CLD_BL2_PIN;
                GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
                GPIO_InitStruct.Pull = GPIO_NOPULL;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
                HAL_GPIO_Init(CLD_BL2_PORT, &GPIO_InitStruct);
            }
            HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_RESET);  /* BL1 LOW = ON */
            HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_RESET);  /* BL2 LOW = dim */
            break;
            
        case 2:  /* High brightness */
        case 3:
        default:
            /* First ensure PB2 is output mode */
            {
                GPIO_InitTypeDef GPIO_InitStruct = {0};
                GPIO_InitStruct.Pin = CLD_BL2_PIN;
                GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
                GPIO_InitStruct.Pull = GPIO_NOPULL;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
                HAL_GPIO_Init(CLD_BL2_PORT, &GPIO_InitStruct);
            }
            HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_RESET);  /* BL1 LOW = ON */
            HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_SET);    /* BL2 HIGH = bright */
            break;
    }
}

void ST7735_RGB_UPDATE(uint8_t R, uint8_t G, uint8_t B)
{
    /* Convert RGB values to 16-bit color (RGB565 format) */
    /* R: 5 bits (0-31), G: 6 bits (0-63), B: 5 bits (0-31) */
    uint16_t color = ((R & 0x1F) << 11) | ((G & 0x3F) << 5) | (B & 0x1F);
    
    /* Fill entire screen with the color */
    ST7735_FillScreen(color);
}