/**
 * @file wb32_hal_like.h
 * @brief WB32L003最小寄存器定义
 * @note 不依赖HAL库的轻量级实现
 */

#ifndef __WB32_HAL_LIKE_H
#define __WB32_HAL_LIKE_H

// 基本类型定义
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

// 基地址定义
#define RCC_BASE        0x40021000UL
#define GPIOA_BASE      0x48000000UL
#define GPIOB_BASE      0x48000400UL
#define GPIOC_BASE      0x48000800UL
#define GPIOD_BASE      0x48000C00UL

// GPIO寄存器结构
typedef struct {
    volatile uint32_t MODER;    // 0x00 模式寄存器
    volatile uint32_t OTYPER;   // 0x04 输出类型寄存器
    volatile uint32_t OSPEEDR;  // 0x08 输出速度寄存器
    volatile uint32_t PUPDR;    // 0x0C 上下拉寄存器
    volatile uint32_t IDR;      // 0x10 输入数据寄存器
    volatile uint32_t ODR;      // 0x14 输出数据寄存器
    volatile uint32_t BSRR;     // 0x18 位设置/复位寄存器
    volatile uint32_t LCKR;     // 0x1C 锁定寄存器
    volatile uint32_t AFR[2];   // 0x20-0x24 复用功能寄存器
    volatile uint32_t BRR;      // 0x28 位复位寄存器
} GPIO_TypeDef;

// RCC寄存器结构
typedef struct {
    volatile uint32_t CR;       // 0x00
    volatile uint32_t CFGR;     // 0x04
    volatile uint32_t CIR;      // 0x08
    volatile uint32_t APB2RSTR; // 0x0C
    volatile uint32_t APB1RSTR; // 0x10
    volatile uint32_t AHBENR;   // 0x14 AHB外设时钟使能
    volatile uint32_t APB2ENR;  // 0x18
    volatile uint32_t APB1ENR;  // 0x1C
} RCC_TypeDef;

// 外设指针定义
#define RCC     ((RCC_TypeDef *)RCC_BASE)
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)GPIOD_BASE)

// RCC时钟使能宏
#define RCC_EnableGPIOA()  (RCC->AHBENR |= (1UL << 17))
#define RCC_EnableGPIOB()  (RCC->AHBENR |= (1UL << 18))
#define RCC_EnableGPIOC()  (RCC->AHBENR |= (1UL << 19))
#define RCC_EnableGPIOD()  (RCC->AHBENR |= (1UL << 20))

// 系统时钟（假设32MHz）
#define SystemCoreClock    32000000UL

// 系统时钟配置（简化版）
static inline void SystemClock_Config(void) {
    // 这里应该配置PLL等，简化处理假设已经是32MHz
    // 实际项目中应该正确配置时钟
}

// NOP指令
#define __NOP()  __asm__ volatile ("nop")

#endif /* __WB32_HAL_LIKE_H */