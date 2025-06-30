#include "st7735.h"

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim2;

static void ST7735_SpiWrite(uint8_t data)
{
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
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
    HAL_Delay(10);
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