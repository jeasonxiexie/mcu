/**
 * @file qfn32_simple.c  
 * @brief 最简单的QFN32测试 - 只做LED闪烁
 * @note 最小改动，最可靠
 */

#include <stdint.h>

#define __IO volatile

// 外设基地址
#define PERIPH_BASE     0x40000000UL
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE (PERIPH_BASE + 0x08000000UL)

// GPIO基地址
#define GPIOA_BASE      (AHB2PERIPH_BASE + 0x00000000UL)
#define GPIOC_BASE      (AHB2PERIPH_BASE + 0x00000800UL)

// RCC基地址
#define RCC_BASE        (AHB1PERIPH_BASE + 0x00001000UL)

// 寄存器结构
typedef struct {
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t LCKR;
    __IO uint32_t AFR[2];
    __IO uint32_t BRR;
} GPIO_TypeDef;

typedef struct {
    __IO uint32_t CR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t APB2RSTR;
    __IO uint32_t APB1RSTR;
    __IO uint32_t AHBENR;
    __IO uint32_t APB2ENR;
    __IO uint32_t APB1ENR;
    __IO uint32_t BDCR;
    __IO uint32_t CSR;
    __IO uint32_t AHBRSTR;
    __IO uint32_t CFGR2;
    __IO uint32_t CFGR3;
    __IO uint32_t CR2;
} RCC_TypeDef;

#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define RCC     ((RCC_TypeDef *)RCC_BASE)

// 简单延时
void delay(uint32_t count) {
    while(count--) {
        for(volatile int i = 0; i < 1000; i++);
    }
}

int main(void) {
    // 1. 使能GPIO时钟
    RCC->AHBENR |= (1UL << 17) | (1UL << 19);  // GPIOA和GPIOC
    
    // 2. 电源控制 - QFN32引脚
    // PC6 (Pin 23) - CPU电源，必须高电平
    GPIOC->MODER &= ~(0x03UL << 12);
    GPIOC->MODER |= (0x01UL << 12);  // 输出模式
    GPIOC->PUPDR &= ~(0x03UL << 12);
    GPIOC->PUPDR |= (0x01UL << 12);  // 上拉
    GPIOC->OSPEEDR |= (0x03UL << 12); // 高速
    GPIOC->BSRR = (1UL << 6);  // 设置高电平
    
    // PA3 (Pin 7) - LCD电源
    GPIOA->MODER &= ~(0x03UL << 6);
    GPIOA->MODER |= (0x01UL << 6);
    GPIOA->PUPDR |= (0x01UL << 6);
    GPIOA->OSPEEDR |= (0x03UL << 6);
    GPIOA->BSRR = (1UL << 3);
    
    // PC5 (Pin 22) - RF电源
    GPIOC->MODER &= ~(0x03UL << 10);
    GPIOC->MODER |= (0x01UL << 10);
    GPIOC->PUPDR |= (0x01UL << 10);
    GPIOC->OSPEEDR |= (0x03UL << 10);
    GPIOC->BSRR = (1UL << 5);
    
    // 3. LED配置 - QFN32引脚
    // PC0 (Pin 17) - 红色LED
    GPIOC->MODER &= ~(0x03UL << 0);
    GPIOC->MODER |= (0x01UL << 0);
    GPIOC->OSPEEDR |= (0x03UL << 0);
    GPIOC->BSRR = (1UL << 0);  // 初始关闭
    
    // PC1 (Pin 18) - 绿色LED
    GPIOC->MODER &= ~(0x03UL << 2);
    GPIOC->MODER |= (0x01UL << 2);
    GPIOC->OSPEEDR |= (0x03UL << 2);
    GPIOC->BSRR = (1UL << 1);  // 初始关闭
    
    // 4. 主循环 - LED交替闪烁
    while(1) {
        // 红灯亮，绿灯灭
        GPIOC->BRR = (1UL << 0);   // PC0低 = 红灯亮
        GPIOC->BSRR = (1UL << 1);  // PC1高 = 绿灯灭
        delay(500);
        
        // 红灯灭，绿灯亮
        GPIOC->BSRR = (1UL << 0);  // PC0高 = 红灯灭
        GPIOC->BRR = (1UL << 1);   // PC1低 = 绿灯亮
        delay(500);
    }
    
    return 0;
}