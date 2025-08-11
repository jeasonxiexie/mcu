/**
 * @file qfn32_test.c
 * @brief WB32L003 QFN-32封装测试程序
 * @note LED交替闪烁 + UART2调试输出
 * 
 * QFN-32引脚映射:
 * - PC0 (Pin 17) - 红色LED (低电平点亮)
 * - PC1 (Pin 18) - 绿色LED (低电平点亮)
 * - PD5 (Pin 30) - UART2_TX
 * - PC6 (Pin 23) - CPU电源控制
 * - PA3 (Pin 7)  - LCD电源控制
 * - PC5 (Pin 22) - RF电源控制
 */

#include <stdint.h>

// 基本寄存器定义
#define __IO volatile

// 外设基地址
#define PERIPH_BASE     0x40000000UL
#define APB1PERIPH_BASE PERIPH_BASE
#define APB2PERIPH_BASE (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE (PERIPH_BASE + 0x08000000UL)

// GPIO基地址
#define GPIOA_BASE      (AHB2PERIPH_BASE + 0x00000000UL)
#define GPIOC_BASE      (AHB2PERIPH_BASE + 0x00000800UL)
#define GPIOD_BASE      (AHB2PERIPH_BASE + 0x00000C00UL)

// RCC基地址
#define RCC_BASE        (AHB1PERIPH_BASE + 0x00001000UL)

// UART2基地址
#define UART2_BASE      (APB1PERIPH_BASE + 0x00004400UL)

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

// UART寄存器结构
typedef struct {
    __IO uint32_t SR;         // 0x00 状态寄存器
    __IO uint32_t DR;         // 0x04 数据寄存器
    __IO uint32_t BRR;        // 0x08 波特率寄存器
    __IO uint32_t CR1;        // 0x0C 控制寄存器1
    __IO uint32_t CR2;        // 0x10 控制寄存器2
    __IO uint32_t CR3;        // 0x14 控制寄存器3
    __IO uint32_t GTPR;       // 0x18 保护时间和预分频寄存器
} UART_TypeDef;

// Flash寄存器结构
typedef struct {
    __IO uint32_t ACR;        // 0x00
} FLASH_TypeDef;

// 外设指针
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)GPIOD_BASE)
#define RCC     ((RCC_TypeDef *)RCC_BASE)
#define UART2   ((UART_TypeDef *)UART2_BASE)
#define FLASH   ((FLASH_TypeDef *)FLASH_R_BASE)

// RCC时钟使能位
#define RCC_AHBENR_GPIOAEN   (1UL << 17)
#define RCC_AHBENR_GPIOCEN   (1UL << 19)
#define RCC_AHBENR_GPIODEN   (1UL << 20)
#define RCC_APB1ENR_UART2EN  (1UL << 17)

// UART状态位
#define UART_SR_TXE    (1UL << 7)   // 发送数据寄存器空
#define UART_SR_TC     (1UL << 6)   // 发送完成
#define UART_CR1_UE    (1UL << 13)  // UART使能
#define UART_CR1_TE    (1UL << 3)   // 发送使能
#define UART_CR1_RE    (1UL << 2)   // 接收使能

// 系统时钟频率 (12MHz HSI)
#define SYSTEM_CLOCK_HZ     12000000UL

// 延时函数
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        for(uint32_t j = 0; j < (SYSTEM_CLOCK_HZ/10000); j++) {
            __asm__("nop");
        }
    }
}

// 系统时钟初始化
void SystemClock_Config(void) {
    // 使能HSI
    RCC->CR |= 0x01;  // HSION = 1
    
    // 等待HSI就绪
    while(!(RCC->CR & 0x02));  // 等待HSIRDY
    
    // 配置Flash延迟
    FLASH->ACR = 0x01;  // 1等待状态
}

// GPIO完整初始化函数
void gpio_init(GPIO_TypeDef* GPIOx, uint32_t pin, 
               uint32_t mode, uint32_t otype, 
               uint32_t speed, uint32_t pupd, uint32_t af) {
    // 配置模式
    GPIOx->MODER &= ~(0x03UL << (pin * 2));
    GPIOx->MODER |= (mode << (pin * 2));
    
    // 配置输出类型
    GPIOx->OTYPER &= ~(0x01UL << pin);
    GPIOx->OTYPER |= (otype << pin);
    
    // 配置速度
    GPIOx->OSPEEDR &= ~(0x03UL << (pin * 2));
    GPIOx->OSPEEDR |= (speed << (pin * 2));
    
    // 配置上拉/下拉
    GPIOx->PUPDR &= ~(0x03UL << (pin * 2));
    GPIOx->PUPDR |= (pupd << (pin * 2));
    
    // 配置复用功能
    if(mode == 0x02) {  // AF模式
        if(pin < 8) {
            GPIOx->AFR[0] &= ~(0x0FUL << (pin * 4));
            GPIOx->AFR[0] |= (af << (pin * 4));
        } else {
            GPIOx->AFR[1] &= ~(0x0FUL << ((pin - 8) * 4));
            GPIOx->AFR[1] |= (af << ((pin - 8) * 4));
        }
    }
}

// 电源控制初始化
void Power_Control_Init(void) {
    // 使能GPIO时钟
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN;
    
    // PC6 - CPU电源控制 (Pin 23) - 必须首先设置！
    gpio_init(GPIOC, 6, 0x01, 0x00, 0x03, 0x01, 0);  // 输出,推挽,高速,上拉
    GPIOC->BSRR = (1UL << 6);  // 设置高电平
    
    // PA3 - LCD电源 (Pin 7)
    gpio_init(GPIOA, 3, 0x01, 0x00, 0x03, 0x01, 0);
    GPIOA->BSRR = (1UL << 3);
    
    // PC5 - RF电源 (Pin 22)
    gpio_init(GPIOC, 5, 0x01, 0x00, 0x03, 0x01, 0);
    GPIOC->BSRR = (1UL << 5);
}

// LED初始化
void LED_Init(void) {
    // 确保GPIOC时钟已使能
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    // PC0 - 红色LED (Pin 17) - 低电平点亮
    gpio_init(GPIOC, 0, 0x01, 0x00, 0x03, 0x00, 0);  // 输出,推挽,高速,无上下拉
    GPIOC->BSRR = (1UL << 0);  // 初始高电平(LED灭)
    
    // PC1 - 绿色LED (Pin 18) - 低电平点亮
    gpio_init(GPIOC, 1, 0x01, 0x00, 0x03, 0x00, 0);
    GPIOC->BSRR = (1UL << 1);  // 初始高电平(LED灭)
}

// UART2初始化
void UART2_Init(void) {
    // 使能GPIOD和UART2时钟
    RCC->AHBENR |= RCC_AHBENR_GPIODEN;
    RCC->APB1ENR |= RCC_APB1ENR_UART2EN;
    
    // PD5配置为UART2_TX (AF0) - Pin 30
    gpio_init(GPIOD, 5, 0x02, 0x00, 0x03, 0x01, 0x00);  // AF模式,推挽,高速,上拉,AF0
    
    // 配置波特率 115200
    // BRR = SystemCoreClock / baud_rate
    // BRR = 12000000 / 115200 = 104
    UART2->BRR = 104;
    
    // 配置UART: 8位数据,1停止位,无校验
    UART2->CR1 = 0;
    UART2->CR2 = 0;
    UART2->CR3 = 0;
    
    // 使能发送和UART
    UART2->CR1 |= UART_CR1_TE | UART_CR1_UE;
}

// UART2发送字节
void UART2_SendByte(uint8_t data) {
    // 等待发送缓冲区空
    while(!(UART2->SR & UART_SR_TXE));
    UART2->DR = data;
}

// UART2发送字符串
void UART2_SendString(const char* str) {
    while(*str) {
        UART2_SendByte(*str++);
    }
}

// 主函数
int main(void) {
    uint8_t led_state = 0;
    uint32_t tick_counter = 0;
    
    // 系统时钟初始化
    SystemClock_Config();
    
    // 第一步：电源控制（必须在150ms内）
    Power_Control_Init();
    
    // 初始化LED
    LED_Init();
    
    // 初始化UART2
    UART2_Init();
    
    // 启动信号：快闪3次
    UART2_SendString("\r\n=== QFN-32 WB32L003 Test Starting ===\r\n");
    for(int i = 0; i < 3; i++) {
        GPIOC->BRR = (1UL << 0) | (1UL << 1);  // 两个LED都亮
        delay_ms(100);
        GPIOC->BSRR = (1UL << 0) | (1UL << 1); // 两个LED都灭
        delay_ms(100);
    }
    
    UART2_SendString("System initialized successfully!\r\n");
    UART2_SendString("LED alternating every 500ms...\r\n");
    
    // 主循环：LED交替闪烁 + UART输出
    while(1) {
        // LED交替闪烁
        if(led_state == 0) {
            GPIOC->BRR = (1UL << 0);   // PC0低电平 - 红灯亮
            GPIOC->BSRR = (1UL << 1);  // PC1高电平 - 绿灯灭
            led_state = 1;
        } else {
            GPIOC->BSRR = (1UL << 0);  // PC0高电平 - 红灯灭
            GPIOC->BRR = (1UL << 1);   // PC1低电平 - 绿灯亮
            led_state = 0;
        }
        
        // 每秒输出一次调试信息
        tick_counter++;
        if(tick_counter >= 2) {  // 500ms * 2 = 1秒
            tick_counter = 0;
            UART2_SendString("[Tick] System running... LED: ");
            if(led_state) {
                UART2_SendString("RED\r\n");
            } else {
                UART2_SendString("GREEN\r\n");
            }
        }
        
        delay_ms(500);
    }
    
    return 0;
}

// 错误处理
void Error_Handler(void) {
    while(1) {
        // 快速闪烁表示错误
        GPIOC->BRR = (1UL << 0) | (1UL << 1);
        delay_ms(100);
        GPIOC->BSRR = (1UL << 0) | (1UL << 1);
        delay_ms(100);
    }
}