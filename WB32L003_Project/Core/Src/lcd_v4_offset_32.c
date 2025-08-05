/**
 * @file lcd_v4_offset_32.c
 * @brief LCD driver variant 4 - 32 pixel X-axis offset
 * @version v4
 */

#include "lcd_mvp.h"

/* Pin control macros */
#define LCD_SCLK_LOW()  HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_RESET)
#define LCD_SCLK_HIGH() HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_SET)
#define LCD_MOSI_LOW()  HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_RESET)
#define LCD_MOSI_HIGH() HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_SET)
#define LCD_CS_LOW()    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()   HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_DC_LOW()    HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH()   HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET)
#define LCD_RST_LOW()   HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)
#define LCD_RST_HIGH()  HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)
#define LCD_BLK_ON()    HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_RESET)
#define LCD_BLK_OFF()   HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_SET)

void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = LCD_RST_PIN | LCD_RS_PIN | LCD_SCL_PIN | 
                         LCD_CS_PIN | LCD_SDA_PIN | CLD_BL1_PIN | CLD_BL2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    LCD_CS_HIGH();
    LCD_SCLK_HIGH();
    LCD_MOSI_HIGH();
    LCD_RST_HIGH();
    LCD_DC_HIGH();
}

void LCD_WriteBus(uint8_t dat)
{
    uint8_t i;
    LCD_CS_LOW();
    
    for(i = 0; i < 8; i++)
    {
        LCD_SCLK_LOW();
        if(dat & 0x80) {
            LCD_MOSI_HIGH();
        } else {
            LCD_MOSI_LOW();
        }
        LCD_SCLK_HIGH();
        dat <<= 1;
    }
    
    LCD_CS_HIGH();
}

void LCD_WriteData8(uint8_t dat)
{
    LCD_WriteBus(dat);
}

void LCD_WriteData16(uint16_t dat)
{
    LCD_WriteBus(dat >> 8);
    LCD_WriteBus(dat);
}

void LCD_WriteCommand(uint8_t cmd)
{
    LCD_DC_LOW();
    LCD_WriteBus(cmd);
    LCD_DC_HIGH();
}

void LCD_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    /* V4: 32 pixel X-axis offset */
    LCD_WriteCommand(0x2A);
    LCD_WriteData8(0x00);
    LCD_WriteData8(x1 + 32);  /* 32 pixel offset */
    LCD_WriteData8(0x00);
    LCD_WriteData8(x2 + 32);  /* 32 pixel offset */
    
    LCD_WriteCommand(0x2B);
    LCD_WriteData8(0x00);
    LCD_WriteData8(y1);
    LCD_WriteData8(0x00);
    LCD_WriteData8(y2);
    
    LCD_WriteCommand(0x2C);
}

void LCD_Init(void)
{
    LCD_GPIO_Init();
    
    LCD_RST_LOW();
    HAL_Delay(100);
    LCD_RST_HIGH();
    HAL_Delay(100);
    
    LCD_BLK_ON();
    HAL_Delay(100);
    
    LCD_WriteCommand(0x11);
    HAL_Delay(120);
    
    LCD_WriteCommand(0xB1);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x3C);
    
    LCD_WriteCommand(0xB2);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x3C);
    
    LCD_WriteCommand(0xB3);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x3C);
    
    LCD_WriteCommand(0xB4);
    LCD_WriteData8(0x03);
    
    LCD_WriteCommand(0xC0);
    LCD_WriteData8(0x0E);
    LCD_WriteData8(0x0E);
    LCD_WriteData8(0x04);
    
    LCD_WriteCommand(0xC1);
    LCD_WriteData8(0xC5);
    
    LCD_WriteCommand(0xC2);
    LCD_WriteData8(0x0D);
    LCD_WriteData8(0x00);
    
    LCD_WriteCommand(0xC3);
    LCD_WriteData8(0x8D);
    LCD_WriteData8(0x2A);
    
    LCD_WriteCommand(0xC4);
    LCD_WriteData8(0x8D);
    LCD_WriteData8(0xEE);
    
    LCD_WriteCommand(0xC5);
    LCD_WriteData8(0x06);
    
    LCD_WriteCommand(0x36);
    LCD_WriteData8(0x08);
    
    LCD_WriteCommand(0x3A);
    LCD_WriteData8(0x55);
    
    LCD_WriteCommand(0xE0);
    LCD_WriteData8(0x0B);
    LCD_WriteData8(0x17);
    LCD_WriteData8(0x0A);
    LCD_WriteData8(0x0D);
    LCD_WriteData8(0x1A);
    LCD_WriteData8(0x19);
    LCD_WriteData8(0x16);
    LCD_WriteData8(0x1D);
    LCD_WriteData8(0x21);
    LCD_WriteData8(0x26);
    LCD_WriteData8(0x37);
    LCD_WriteData8(0x3C);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x09);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x10);
    
    LCD_WriteCommand(0xE1);
    LCD_WriteData8(0x0C);
    LCD_WriteData8(0x19);
    LCD_WriteData8(0x09);
    LCD_WriteData8(0x0D);
    LCD_WriteData8(0x1B);
    LCD_WriteData8(0x19);
    LCD_WriteData8(0x15);
    LCD_WriteData8(0x1D);
    LCD_WriteData8(0x21);
    LCD_WriteData8(0x26);
    LCD_WriteData8(0x39);
    LCD_WriteData8(0x3E);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x09);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x10);
    
    HAL_Delay(120);
    
    LCD_WriteCommand(0x29);
}

void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    LCD_SetAddress(xsta, ysta, xend - 1, yend - 1);
    
    for(i = ysta; i < yend; i++)
    {
        for(j = xsta; j < xend; j++)
        {
            LCD_WriteData16(color);
        }
    }
}

void LCD_Clear(uint16_t color)
{
    LCD_Fill(0, 0, LCD_W, LCD_H, color);
}