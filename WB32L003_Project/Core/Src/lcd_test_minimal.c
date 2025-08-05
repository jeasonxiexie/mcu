/**
 * @file lcd_test_minimal.c
 * @brief Minimal LCD test to verify basic functionality
 * @version 8.0 - Focus on LCD only, avoid pin conflicts
 */

#include "main.h"
#include "pin_config.h"

/* Basic delay function */
static void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < ms; i++) {
        for(volatile uint32_t j = 0; j < 2000; j++) {
            __NOP();
        }
    }
}

/* GPIO control macros */
#define LCD_CS_LOW()    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()   HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_RS_LOW()    HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET)
#define LCD_RS_HIGH()   HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET)
#define LCD_RST_LOW()   HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)
#define LCD_RST_HIGH()  HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)
#define LCD_SCL_LOW()   HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_RESET)
#define LCD_SCL_HIGH()  HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_SET)
#define LCD_SDA_LOW()   HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_RESET)
#define LCD_SDA_HIGH()  HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_SET)

/* Software SPI write byte */
static void SPI_WriteByte(uint8_t data)
{
    for(int i = 7; i >= 0; i--) {
        LCD_SCL_LOW();
        if(data & (1 << i)) {
            LCD_SDA_HIGH();
        } else {
            LCD_SDA_LOW();
        }
        LCD_SCL_HIGH();
    }
}

/* Write command to LCD */
static void LCD_WriteCommand(uint8_t cmd)
{
    LCD_CS_LOW();
    LCD_RS_LOW();  // Command mode
    SPI_WriteByte(cmd);
    LCD_CS_HIGH();
}

/* Write data to LCD */
static void LCD_WriteData(uint8_t data)
{
    LCD_CS_LOW();
    LCD_RS_HIGH();  // Data mode
    SPI_WriteByte(data);
    LCD_CS_HIGH();
}

/* Basic LCD initialization */
static void LCD_Init(void)
{
    /* Hardware reset */
    LCD_RST_HIGH();
    delay_ms(10);
    LCD_RST_LOW();
    delay_ms(10);
    LCD_RST_HIGH();
    delay_ms(120);
    
    /* Sleep out */
    LCD_WriteCommand(0x11);
    delay_ms(120);
    
    /* Display on */
    LCD_WriteCommand(0x29);
    delay_ms(10);
    
    /* Set column address */
    LCD_WriteCommand(0x2A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);  // Start column
    LCD_WriteData(0x00);
    LCD_WriteData(0x4F);  // End column (79)
    
    /* Set row address */
    LCD_WriteCommand(0x2B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);  // Start row
    LCD_WriteData(0x00);
    LCD_WriteData(0x9F);  // End row (159)
    
    /* Memory write */
    LCD_WriteCommand(0x2C);
}

/* Fill screen with color */
static void LCD_FillScreen(uint16_t color)
{
    /* Set window to full screen */
    LCD_WriteCommand(0x2A);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x4F);
    
    LCD_WriteCommand(0x2B);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x9F);
    
    /* Write pixels */
    LCD_WriteCommand(0x2C);
    
    for(uint32_t i = 0; i < 80 * 160; i++) {
        LCD_WriteData(color >> 8);
        LCD_WriteData(color & 0xFF);
    }
}

/* Simple system clock configuration */
static void SystemClock_Config(void)
{
    /* Default HSI clock is already configured by SystemInit() */
}

int main(void)
{
    /* HAL initialization */
    HAL_Init();
    
    /* Configure system clock */
    SystemClock_Config();
    
    /* Enable GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Configure LCD control pins - ALL ON GPIOC! */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    /* LCD pins on GPIOC */
    GPIO_InitStruct.Pin = LCD_RST_PIN | LCD_RS_PIN | LCD_SCL_PIN | 
                         LCD_CS_PIN | LCD_SDA_PIN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /* Set initial states */
    LCD_CS_HIGH();
    LCD_RST_HIGH();
    LCD_SCL_HIGH();
    
    /* Configure LCD power control - PA3 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* Enable LCD power (LOW = ON for PNP transistor) */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    delay_ms(50);
    
    /* Configure backlight pins - PB1, PB2 */
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    /* Enable backlight (LOW = ON for PNP transistor) */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);  // BL1 ON
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);  // BL2 ON
    
    /* Initialize LCD */
    LCD_Init();
    
    /* Test pattern - cycle through colors */
    while(1) {
        LCD_FillScreen(0xF800);  // Red
        delay_ms(1000);
        
        LCD_FillScreen(0x07E0);  // Green
        delay_ms(1000);
        
        LCD_FillScreen(0x001F);  // Blue
        delay_ms(1000);
        
        LCD_FillScreen(0xFFFF);  // White
        delay_ms(1000);
        
        LCD_FillScreen(0x0000);  // Black
        delay_ms(1000);
    }
}