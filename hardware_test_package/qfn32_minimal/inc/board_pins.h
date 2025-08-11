/**
 * @file board_pins.h
 * @brief WB32L003 QFN32 引脚定义
 * @note 经过硬件验证的引脚映射
 */

#ifndef __BOARD_PINS_H
#define __BOARD_PINS_H

// LED引脚定义（低电平点亮）
#define LED_R_PORT   GPIOC
#define LED_R_PIN    0      // PC0 = LED_RED (Pin 17)

#define LED_G_PORT   GPIOC  
#define LED_G_PIN    1      // PC1 = LED_GREEN (Pin 18)

// 电源控制引脚（高电平使能）
#define POW_CPU_PORT GPIOC  // PC6 = CON_POW_CPU (Pin 23)
#define POW_CPU_PIN  6

#define POW_LCD_PORT GPIOA  // PA3 = CON_POW_LCD (Pin 7)
#define POW_LCD_PIN  3

#define POW_RF_PORT  GPIOC  // PC5 = CON_POW_RF (Pin 22)
#define POW_RF_PIN   5

// 调试串口引脚
#define DBG_TX_PORT  GPIOB  // PB4 -> USB-UART RX (Pin 12)
#define DBG_TX_PIN   4

// 串口配置
#define BAUD         38400u // 起步用38400更稳定
#define BIT_US       (1000000u/BAUD)

#endif /* __BOARD_PINS_H */