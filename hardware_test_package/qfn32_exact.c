/**
 * @file qfn32_exact.c
 * @brief 完全模拟TIM10_Count的启动流程，只改引脚
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
#define FLASH_R_BASE    (AHB1PERIPH_BASE + 0x00002000UL)

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

typedef struct {
    __IO uint32_t ACR;
} FLASH_TypeDef;

#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define RCC     ((RCC_TypeDef *)RCC_BASE)
#define FLASH   ((FLASH_TypeDef *)FLASH_R_BASE)

// 系统时钟变量（模拟SystemCoreClock）
uint32_t SystemCoreClock = 8000000;  // 默认8MHz HSI

// 模拟SystemCoreClockUpdate函数
void SystemCoreClockUpdate(void) {
    // 简化版本，假设使用HSI
    SystemCoreClock = 8000000;
}

// 模拟TIM10_Count中使用的delay
void delay(uint32_t ticks) {
    while (ticks--)
        ;
}

// 这个函数确保PC6首先被设置为高电平
void EarlyInit(void) {
    // 立即使能GPIOC时钟
    *((volatile uint32_t*)(RCC_BASE + 0x14)) |= (1UL << 19);
    
    // 配置PC6为输出
    *((volatile uint32_t*)(GPIOC_BASE + 0x00)) &= ~(0x03UL << 12);
    *((volatile uint32_t*)(GPIOC_BASE + 0x00)) |= (0x01UL << 12);
    
    // PC6设置为高电平（必须在150ms内）
    *((volatile uint32_t*)(GPIOC_BASE + 0x18)) = (1UL << 6);
}

int main(void) {
    // 尽早锁存电源
    EarlyInit();
    
    // 模拟SystemCoreClockUpdate
    SystemCoreClockUpdate();
    
    // 完整的时钟初始化
    RCC->CR |= 0x01;  // HSION
    while(!(RCC->CR & 0x02));  // 等待HSIRDY
    
    // Flash延迟
    FLASH->ACR = 0x01;
    
    // 使能所需的GPIO时钟
    RCC->AHBENR |= (1UL << 17) | (1UL << 19);  // GPIOA和GPIOC
    
    // 延时确保时钟稳定
    for(volatile int i = 0; i < 1000; i++);
    
    // ===== 电源控制引脚 =====
    // PC6已经在EarlyInit中设置
    // 但这里再次确保配置正确
    GPIOC->MODER &= ~(0x03UL << 12);
    GPIOC->MODER |= (0x01UL << 12);  // 输出模式
    GPIOC->OTYPER &= ~(0x01UL << 6); // 推挽
    GPIOC->OSPEEDR &= ~(0x03UL << 12);
    GPIOC->OSPEEDR |= (0x03UL << 12); // 高速
    GPIOC->PUPDR &= ~(0x03UL << 12);
    GPIOC->PUPDR |= (0x01UL << 12);  // 上拉
    GPIOC->BSRR = (1UL << 6);  // 保持高电平
    
    // PA3 - LCD电源
    GPIOA->MODER &= ~(0x03UL << 6);
    GPIOA->MODER |= (0x01UL << 6);
    GPIOA->OTYPER &= ~(0x01UL << 3);
    GPIOA->OSPEEDR &= ~(0x03UL << 6);
    GPIOA->OSPEEDR |= (0x03UL << 6);
    GPIOA->PUPDR &= ~(0x03UL << 6);
    GPIOA->PUPDR |= (0x01UL << 6);
    GPIOA->BSRR = (1UL << 3);
    
    // PC5 - RF电源
    GPIOC->MODER &= ~(0x03UL << 10);
    GPIOC->MODER |= (0x01UL << 10);
    GPIOC->OTYPER &= ~(0x01UL << 5);
    GPIOC->OSPEEDR &= ~(0x03UL << 10);
    GPIOC->OSPEEDR |= (0x03UL << 10);
    GPIOC->PUPDR &= ~(0x03UL << 10);
    GPIOC->PUPDR |= (0x01UL << 10);
    GPIOC->BSRR = (1UL << 5);
    
    // ===== LED配置 =====
    // PC0 - 红色LED
    GPIOC->MODER &= ~(0x03UL << 0);
    GPIOC->MODER |= (0x01UL << 0);
    GPIOC->OTYPER &= ~(0x01UL << 0);
    GPIOC->OSPEEDR &= ~(0x03UL << 0);
    GPIOC->OSPEEDR |= (0x03UL << 0);
    GPIOC->PUPDR &= ~(0x03UL << 0);  // 无上下拉
    GPIOC->BSRR = (1UL << 0);  // 初始关闭
    
    // PC1 - 绿色LED
    GPIOC->MODER &= ~(0x03UL << 2);
    GPIOC->MODER |= (0x01UL << 2);
    GPIOC->OTYPER &= ~(0x01UL << 1);
    GPIOC->OSPEEDR &= ~(0x03UL << 2);
    GPIOC->OSPEEDR |= (0x03UL << 2);
    GPIOC->PUPDR &= ~(0x03UL << 2);  // 无上下拉
    GPIOC->BSRR = (1UL << 1);  // 初始关闭
    
    // 启动时快闪3次表示系统运行
    for(int i = 0; i < 3; i++) {
        GPIOC->BRR = (1UL << 0) | (1UL << 1);  // 两个LED都亮
        delay(1000000);
        GPIOC->BSRR = (1UL << 0) | (1UL << 1); // 两个LED都灭
        delay(1000000);
    }
    
    // 主循环 - LED交替闪烁
    while(1) {
        // 红灯亮，绿灯灭
        GPIOC->BRR = (1UL << 0);   // PC0低
        GPIOC->BSRR = (1UL << 1);  // PC1高
        delay(3000000);  // 使用TIM10_Count相同的延时方式
        
        // 红灯灭，绿灯亮
        GPIOC->BSRR = (1UL << 0);  // PC0高
        GPIOC->BRR = (1UL << 1);   // PC1低
        delay(3000000);
    }
    
    return 0;
}