/**
 * @file test_toggle.c
 * @brief WB32L003 PC6引脚切换测试
 * @note 专门用于测试PC6能否输出方波信号
 */

#include <stdint.h>

// 基本寄存器定义
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

// Flash接口基地址
#define FLASH_R_BASE    (AHB1PERIPH_BASE + 0x00002000UL)

// GPIO寄存器结构
typedef struct {
    __IO uint32_t MODER;    // 0x00 模式寄存器
    __IO uint32_t OTYPER;   // 0x04 输出类型寄存器
    __IO uint32_t OSPEEDR;  // 0x08 输出速度寄存器
    __IO uint32_t PUPDR;    // 0x0C 上拉/下拉寄存器
    __IO uint32_t IDR;      // 0x10 输入数据寄存器
    __IO uint32_t ODR;      // 0x14 输出数据寄存器
    __IO uint32_t BSRR;     // 0x18 位设置/复位寄存器
    __IO uint32_t LCKR;     // 0x1C 配置锁定寄存器
    __IO uint32_t AFR[2];   // 0x20-0x24 复用功能寄存器
    __IO uint32_t BRR;      // 0x28 位复位寄存器
} GPIO_TypeDef;

// RCC寄存器结构
typedef struct {
    __IO uint32_t CR;         // 0x00
    __IO uint32_t CFGR;       // 0x04
    __IO uint32_t CIR;        // 0x08
    __IO uint32_t APB2RSTR;   // 0x0C
    __IO uint32_t APB1RSTR;   // 0x10
    __IO uint32_t AHBENR;     // 0x14
    __IO uint32_t APB2ENR;    // 0x18
    __IO uint32_t APB1ENR;    // 0x1C
    __IO uint32_t BDCR;       // 0x20
    __IO uint32_t CSR;        // 0x24
    __IO uint32_t AHBRSTR;    // 0x28
    __IO uint32_t CFGR2;      // 0x2C
    __IO uint32_t CFGR3;      // 0x30
    __IO uint32_t CR2;        // 0x34
} RCC_TypeDef;

// Flash寄存器结构
typedef struct {
    __IO uint32_t ACR;        // 0x00
} FLASH_TypeDef;

// 外设指针
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define RCC     ((RCC_TypeDef *)RCC_BASE)
#define FLASH   ((FLASH_TypeDef *)FLASH_R_BASE)

// GPIO引脚定义
#define GPIO_PIN_0    0x0001
#define GPIO_PIN_1    0x0002
#define GPIO_PIN_3    0x0008
#define GPIO_PIN_5    0x0020
#define GPIO_PIN_6    0x0040

// RCC时钟使能位
#define RCC_AHBENR_GPIOAEN   (1UL << 17)
#define RCC_AHBENR_GPIOCEN   (1UL << 19)

// 简单延时（用于产生不同频率）
void delay_cycles(uint32_t cycles) {
    while(cycles--) {
        __asm__("nop");
    }
}

// 系统时钟配置
void SystemClock_Config(void) {
    // 使能HSI
    RCC->CR |= 0x01;  // HSION = 1
    
    // 等待HSI就绪
    while(!(RCC->CR & 0x02));  // 等待HSIRDY
    
    // 配置Flash延迟
    FLASH->ACR = 0x01;  // 1等待状态
}

// 配置GPIO为输出
void GPIO_Config(void) {
    // 使能GPIOC时钟
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    // 使能GPIOA时钟
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    
    // ===== 配置PC6（主测试引脚） =====
    // MODER: 01 = 输出模式
    GPIOC->MODER &= ~(0x03UL << 12);
    GPIOC->MODER |= (0x01UL << 12);
    
    // OTYPER: 0 = 推挽输出
    GPIOC->OTYPER &= ~(0x01UL << 6);
    
    // OSPEEDR: 11 = 高速
    GPIOC->OSPEEDR &= ~(0x03UL << 12);
    GPIOC->OSPEEDR |= (0x03UL << 12);
    
    // PUPDR: 01 = 上拉
    GPIOC->PUPDR &= ~(0x03UL << 12);
    GPIOC->PUPDR |= (0x01UL << 12);
    
    // ===== 配置PC0/PC1（LED） =====
    // PC0
    GPIOC->MODER &= ~(0x03UL << 0);
    GPIOC->MODER |= (0x01UL << 0);
    GPIOC->OTYPER &= ~(0x01UL << 0);
    GPIOC->OSPEEDR |= (0x03UL << 0);
    
    // PC1
    GPIOC->MODER &= ~(0x03UL << 2);
    GPIOC->MODER |= (0x01UL << 2);
    GPIOC->OTYPER &= ~(0x01UL << 1);
    GPIOC->OSPEEDR |= (0x03UL << 2);
    
    // ===== 配置其他电源控制引脚 =====
    // PA3 - LCD电源
    GPIOA->MODER &= ~(0x03UL << 6);
    GPIOA->MODER |= (0x01UL << 6);
    GPIOA->OTYPER &= ~(0x01UL << 3);
    GPIOA->OSPEEDR |= (0x03UL << 6);
    GPIOA->PUPDR |= (0x01UL << 6);
    GPIOA->BSRR = GPIO_PIN_3;  // 设置高电平
    
    // PC5 - RF电源
    GPIOC->MODER &= ~(0x03UL << 10);
    GPIOC->MODER |= (0x01UL << 10);
    GPIOC->OTYPER &= ~(0x01UL << 5);
    GPIOC->OSPEEDR |= (0x03UL << 10);
    GPIOC->PUPDR |= (0x01UL << 10);
    GPIOC->BSRR = GPIO_PIN_5;  // 设置高电平
}

// 主函数 - 测试不同的切换模式
int main(void) {
    // 系统初始化
    SystemClock_Config();
    
    // GPIO配置
    GPIO_Config();
    
    // 初始状态：PC6设为高（保持电源）
    GPIOC->BSRR = GPIO_PIN_6;
    
    // 延时100ms确保系统稳定
    for(uint32_t i = 0; i < 100000; i++) {
        delay_cycles(100);
    }
    
    // ========== 测试方案1：快速切换PC6产生方波 ==========
    // 频率约1kHz（可用示波器测量）
    uint32_t counter = 0;
    while(1) {
        // 每1000次循环切换一次LED，便于观察程序运行
        if(++counter >= 1000) {
            counter = 0;
            // 切换LED
            if(GPIOC->ODR & GPIO_PIN_0) {
                GPIOC->BRR = GPIO_PIN_0 | GPIO_PIN_1;
            } else {
                GPIOC->BSRR = GPIO_PIN_0 | GPIO_PIN_1;
            }
        }
        
        // PC6快速切换（产生方波）
        GPIOC->BSRR = GPIO_PIN_6;  // 设置高
        delay_cycles(500);          // 延时
        GPIOC->BRR = GPIO_PIN_6;    // 设置低
        delay_cycles(500);          // 延时
    }
    
    /* ========== 备选测试方案2：慢速切换 ==========
    // 如果担心快速切换影响电源，可以用这个慢速版本
    while(1) {
        // PC6高电平500ms
        GPIOC->BSRR = GPIO_PIN_6;
        for(uint32_t i = 0; i < 500000; i++) {
            delay_cycles(1000);
        }
        
        // PC6低电平500ms
        GPIOC->BRR = GPIO_PIN_6;
        for(uint32_t i = 0; i < 500000; i++) {
            delay_cycles(1000);
        }
        
        // 同时切换LED指示
        if(GPIOC->ODR & GPIO_PIN_0) {
            GPIOC->BRR = GPIO_PIN_0 | GPIO_PIN_1;
        } else {
            GPIOC->BSRR = GPIO_PIN_0 | GPIO_PIN_1;
        }
    }
    */
    
    /* ========== 备选测试方案3：只测试LED ==========
    // 如果不想动PC6，只测试LED是否工作
    while(1) {
        // 切换LED
        if(GPIOC->ODR & GPIO_PIN_0) {
            GPIOC->BRR = GPIO_PIN_0 | GPIO_PIN_1;
        } else {
            GPIOC->BSRR = GPIO_PIN_0 | GPIO_PIN_1;
        }
        
        // 延时500ms
        for(uint32_t i = 0; i < 500000; i++) {
            delay_cycles(1000);
        }
    }
    */
    
    return 0;
}