/**
 * @file config.h
 * @brief 硬件配置文件 - 请根据实际硬件修改
 * @note 修改后重新编译：双击build.bat
 */

#ifndef __CONFIG_H
#define __CONFIG_H

// ============================================================
//                     电源控制配置（最重要！）
// ============================================================
// QFN32版本需要控制三路电源
// PC6 = CON_POW_CPU (Pin 23) - 主电源控制
#define POWER_CPU_PORT      'C'     // 端口
#define POWER_CPU_PIN       6       // Pin 23
#define POWER_CPU_LEVEL     1       // 1=高电平使能

// PA3 = CON_POW_LCD (Pin 7) - LCD电源控制
#define POWER_LCD_PORT      'A'     // 端口
#define POWER_LCD_PIN       3       // Pin 7
#define POWER_LCD_LEVEL     1       // 1=高电平使能

// PC5 = CON_POW_RF (Pin 22) - RF电源控制  
#define POWER_RF_PORT       'C'     // 端口
#define POWER_RF_PIN        5       // Pin 22
#define POWER_RF_LEVEL      1       // 1=高电平使能

// ============================================================
//                        LED配置
// ============================================================
// PC0 = LED_RED (Pin 17) - 红色LED
#define LED1_PORT           'C'     // 端口
#define LED1_PIN            0       // Pin 17
  
// PC1 = LED_GREEN (Pin 18) - 绿色LED
#define LED2_PORT           'C'     // 端口
#define LED2_PIN            1       // Pin 18

// LED极性设置
#define LED_ACTIVE_LOW      1       // 1=低电平点亮(确认), 0=高电平点亮

// ============================================================
//                       串口调试配置
// ============================================================
#define UART_ENABLE         1       // 1=启用串口输出, 0=禁用
#define UART_MODE           1       // 0=硬件UART2, 1=软件模拟

// 硬件UART2配置 (PD5 = Pin 30)
#define UART2_TX_PORT       'D'     // 串口TX端口
#define UART2_TX_PIN        5       // Pin 30
#define UART_BAUDRATE       115200  // 波特率

// 软件模拟串口配置 (使用任意GPIO)
#define SOFT_TX_PORT        'B'     // 可选PB4或其他空闲引脚
#define SOFT_TX_PIN         4       // Pin 12

// ============================================================
//                       系统配置
// ============================================================
#define SYSTEM_CLOCK_HZ     32000000UL  // 32MHz系统时钟
#define DELAY_FACTOR        4000        // 延时系数（根据时钟调整）

// ============================================================
//                    寄存器基地址（不要修改）
// ============================================================
#define RCC_BASE            0x40021000UL
#define GPIOA_BASE          0x48000000UL
#define GPIOB_BASE          0x48000400UL
#define GPIOC_BASE          0x48000800UL
#define GPIOD_BASE          0x48000C00UL
#define USART1_BASE         0x40013800UL
#define USART2_BASE         0x40004400UL

// ============================================================
//                    辅助宏定义（不要修改）
// ============================================================
// GPIO端口选择
#define GPIO_BASE(port) \
    ((port) == 'A' ? GPIOA_BASE : \
     (port) == 'B' ? GPIOB_BASE : \
     (port) == 'C' ? GPIOC_BASE : \
     (port) == 'D' ? GPIOD_BASE : 0)

// RCC时钟使能位
#define RCC_GPIO_ENABLE_BIT(port) \
    ((port) == 'A' ? 17 : \
     (port) == 'B' ? 18 : \
     (port) == 'C' ? 19 : \
     (port) == 'D' ? 20 : 0)

#endif /* __CONFIG_H */