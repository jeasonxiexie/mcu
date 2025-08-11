#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "wb32l003.h"

// LCD尺寸定义
#define USE_HORIZONTAL 0  // 0/1为竖屏，2/3为横屏

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 80
#define LCD_H 160
#else
#define LCD_W 160
#define LCD_H 80
#endif

// LCD接口引脚定义 - 根据原理图
// GPIOC引脚
#define LCD_SCLK_PIN    GPIO_PIN_5   // PC5 - SPI时钟
#define LCD_SCLK_PORT   GPIOC
#define LCD_MOSI_PIN    GPIO_PIN_6   // PC6 - SPI数据
#define LCD_MOSI_PORT   GPIOC
#define LCD_CS_PIN      GPIO_PIN_0   // PC0 - 片选
#define LCD_CS_PORT     GPIOC
#define LCD_DC_PIN      GPIO_PIN_1   // PC1 - 数据/命令
#define LCD_DC_PORT     GPIOC
#define LCD_RES_PIN     GPIO_PIN_3   // PC3 - 复位
#define LCD_RES_PORT    GPIOC

// 背光控制引脚 - GPIOB
#define LCD_BL1_PIN     GPIO_PIN_1   // PB1 - 高亮度(通过PNP三极管，低电平有效)
#define LCD_BL1_PORT    GPIOB
#define LCD_BL2_PIN     GPIO_PIN_2   // PB2 - 低亮度(直接驱动)
#define LCD_BL2_PORT    GPIOB

// LCD电源控制 - GPIOA
#define LCD_PWR_PIN     GPIO_PIN_3   // PA3 - LCD电源使能
#define LCD_PWR_PORT    GPIOA

// 引脚操作宏定义
#define LCD_SCLK_Clr()  HAL_GPIO_WritePin(LCD_SCLK_PORT, LCD_SCLK_PIN, GPIO_PIN_RESET)
#define LCD_SCLK_Set()  HAL_GPIO_WritePin(LCD_SCLK_PORT, LCD_SCLK_PIN, GPIO_PIN_SET)

#define LCD_MOSI_Clr()  HAL_GPIO_WritePin(LCD_MOSI_PORT, LCD_MOSI_PIN, GPIO_PIN_RESET)
#define LCD_MOSI_Set()  HAL_GPIO_WritePin(LCD_MOSI_PORT, LCD_MOSI_PIN, GPIO_PIN_SET)

#define LCD_RES_Clr()   HAL_GPIO_WritePin(LCD_RES_PORT, LCD_RES_PIN, GPIO_PIN_RESET)
#define LCD_RES_Set()   HAL_GPIO_WritePin(LCD_RES_PORT, LCD_RES_PIN, GPIO_PIN_SET)

#define LCD_DC_Clr()    HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_Set()    HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET)

#define LCD_CS_Clr()    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_Set()    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)

#define LCD_BL1_Clr()   HAL_GPIO_WritePin(LCD_BL1_PORT, LCD_BL1_PIN, GPIO_PIN_RESET)  // 低电平点亮
#define LCD_BL1_Set()   HAL_GPIO_WritePin(LCD_BL1_PORT, LCD_BL1_PIN, GPIO_PIN_SET)

#define LCD_BL2_Clr()   HAL_GPIO_WritePin(LCD_BL2_PORT, LCD_BL2_PIN, GPIO_PIN_RESET)
#define LCD_BL2_Set()   HAL_GPIO_WritePin(LCD_BL2_PORT, LCD_BL2_PIN, GPIO_PIN_SET)

#define LCD_PWR_On()    HAL_GPIO_WritePin(LCD_PWR_PORT, LCD_PWR_PIN, GPIO_PIN_SET)    // 高电平使能
#define LCD_PWR_Off()   HAL_GPIO_WritePin(LCD_PWR_PORT, LCD_PWR_PIN, GPIO_PIN_RESET)

// 函数声明
void LCD_GPIO_Init(void);
void LCD_Writ_Bus(uint8_t dat);
void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA(uint16_t dat);
void LCD_WR_REG(uint8_t dat);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Init(void);
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);

// 常用颜色定义
#define WHITE   0xFFFF
#define BLACK   0x0000
#define BLUE    0x001F
#define BRED    0XF81F
#define GRED    0XFFE0
#define GBLUE   0X07FF
#define RED     0xF800
#define MAGENTA 0xF81F
#define GREEN   0x07E0
#define CYAN    0x7FFF
#define YELLOW  0xFFE0
#define BROWN   0XBC40
#define BRRED   0XFC07
#define GRAY    0X8430

#endif