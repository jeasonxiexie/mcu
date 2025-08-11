#ifndef __LCD_INIT_CORRECT_H
#define __LCD_INIT_CORRECT_H

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

// ========== 引脚映射（根据RTF文档） ==========

// LCD接口引脚 - GPIOB（不是GPIOC！）
// 按 docs 使用 PB7 作为硬件复位
#define LCD_RST_PIN     GPIO_PIN_7   // PB7 - LCD_RESET
#define LCD_RST_PORT    GPIOB
#define LCD_RS_PIN      GPIO_PIN_6   // PB6 - LCD_RS (数据/命令)
#define LCD_RS_PORT     GPIOB
#define LCD_SCL_PIN     GPIO_PIN_5   // PB5 - LCD_SCL (时钟)
#define LCD_SCL_PORT    GPIOB
#define LCD_CS_PIN      GPIO_PIN_4   // PB4 - LCD_CS (片选)
#define LCD_CS_PORT     GPIOB
#define LCD_SDA_PIN     GPIO_PIN_3   // PB3 - LCD_SDA (数据)
#define LCD_SDA_PORT    GPIOB

// 背光控制引脚 - GPIOB
#define LCD_BL1_PIN     GPIO_PIN_1   // PB1 - CLD_BL1 (背光-高)
#define LCD_BL1_PORT    GPIOB
#define LCD_BL2_PIN     GPIO_PIN_2   // PB2 - CLD_BL2 (背光-低)
#define LCD_BL2_PORT    GPIOB

// 电源控制引脚
#define LCD_PWR_PIN     GPIO_PIN_3   // PA3 - CON_POW_LCD (LCD电源)
#define LCD_PWR_PORT    GPIOA
#define CPU_PWR_PIN     GPIO_PIN_6   // PC6 - CON_POW_CPU (主电源, HIGH=开)
#define CPU_PWR_PORT    GPIOC
#define RF_PWR_PIN      GPIO_PIN_5   // PC5 - CON_POW_RF (5V升压)
#define RF_PWR_PORT     GPIOC

// 用户界面引脚
#define SW_POW_PIN      GPIO_PIN_0   // PB0 - SW_POW (电源开关)
#define SW_POW_PORT     GPIOB
// KEY_STEREO_PIN removed - PD3 doesn't exist on WB32L003
#define LED_RED_PIN     GPIO_PIN_0   // PC0 - 红色LED (低电平点亮)
#define LED_RED_PORT    GPIOC
#define LED_GREEN_PIN   GPIO_PIN_1   // PC1 - 绿色LED (低电平点亮)
#define LED_GREEN_PORT  GPIOC

// 音频控制引脚
#define CON_MUTE_PIN    GPIO_PIN_4   // PA4 - CON_MUTE (静音控制)
#define CON_MUTE_PORT   GPIOA
// CON_STEREO_PIN removed - PD2 doesn't exist on WB32L003

// 引脚操作宏定义
#define LCD_SCL_Clr()   HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_RESET)
#define LCD_SCL_Set()   HAL_GPIO_WritePin(LCD_SCL_PORT, LCD_SCL_PIN, GPIO_PIN_SET)

#define LCD_SDA_Clr()   HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_RESET)
#define LCD_SDA_Set()   HAL_GPIO_WritePin(LCD_SDA_PORT, LCD_SDA_PIN, GPIO_PIN_SET)

#define LCD_RST_Clr()   HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)
#define LCD_RST_Set()   HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)

#define LCD_RS_Clr()    HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET)
#define LCD_RS_Set()    HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET)

#define LCD_CS_Clr()    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_Set()    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)

// 背光控制宏（与 docs 对齐）
// PB1: HIGH=高亮, LOW=低亮（Q7 供电侧）
// PB2: LOW=使能背光, HIGH=高亮模式
#define LCD_BL1_High()           HAL_GPIO_WritePin(LCD_BL1_PORT, LCD_BL1_PIN, GPIO_PIN_SET)
#define LCD_BL1_Low()            HAL_GPIO_WritePin(LCD_BL1_PORT, LCD_BL1_PIN, GPIO_PIN_RESET)

#define LCD_BL2_Enable()         HAL_GPIO_WritePin(LCD_BL2_PORT, LCD_BL2_PIN, GPIO_PIN_RESET)
#define LCD_BL2_HighBrightness() HAL_GPIO_WritePin(LCD_BL2_PORT, LCD_BL2_PIN, GPIO_PIN_SET)

// 电源控制宏
#define LCD_PWR_On()    HAL_GPIO_WritePin(LCD_PWR_PORT, LCD_PWR_PIN, GPIO_PIN_SET)    // HIGH=开
#define LCD_PWR_Off()   HAL_GPIO_WritePin(LCD_PWR_PORT, LCD_PWR_PIN, GPIO_PIN_RESET)  // LOW=关

#define CPU_PWR_On()    HAL_GPIO_WritePin(CPU_PWR_PORT, CPU_PWR_PIN, GPIO_PIN_SET)    // HIGH=开
#define CPU_PWR_Off()   HAL_GPIO_WritePin(CPU_PWR_PORT, CPU_PWR_PIN, GPIO_PIN_RESET)  // LOW=关

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