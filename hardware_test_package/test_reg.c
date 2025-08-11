/**
 * @file test_reg.c
 * @brief WB32L003 测试程序 - 改进的寄存器直接操作版本
 * @note 基于TIM10_Count成功案例，添加所有缺失的寄存器配置
 */

#include <stdint.h>

// 基本类型定义
#define __IO volatile

// 外设基地址
#define PERIPH_BASE     0x40000000UL
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE (PERIPH_BASE + 0x08000000UL)

// GPIO基地址
#define GPIOA_BASE      (AHB2PERIPH_BASE + 0x00000000UL)
#define GPIOB_BASE      (AHB2PERIPH_BASE + 0x00000400UL)
#define GPIOC_BASE      (AHB2PERIPH_BASE + 0x00000800UL)
#define GPIOD_BASE      (AHB2PERIPH_BASE + 0x00000C00UL)

// RCC基地址
#define RCC_BASE        (AHB1PERIPH_BASE + 0x00001000UL)

// Flash接口基地址
#define FLASH_R_BASE    (AHB1PERIPH_BASE + 0x00002000UL)

// GPIO寄存器结构（完整版）
typedef struct {
    __IO uint32_t MODER;    // 0x00 模式寄存器
    __IO uint32_t OTYPER;   // 0x04 输出类型寄存器
    __IO uint32_t OSPEEDR;  // 0x08 输出速度寄存器 [重要！之前缺失]
    __IO uint32_t PUPDR;    // 0x0C 上拉/下拉寄存器 [重要！之前缺失]
    __IO uint32_t IDR;      // 0x10 输入数据寄存器
    __IO uint32_t ODR;      // 0x14 输出数据寄存器
    __IO uint32_t BSRR;     // 0x18 位设置/复位寄存器
    __IO uint32_t LCKR;     // 0x1C 配置锁定寄存器
    __IO uint32_t AFR[2];   // 0x20-0x24 复用功能寄存器 [重要！之前缺失]
    __IO uint32_t BRR;      // 0x28 位复位寄存器
} GPIO_TypeDef;

// RCC寄存器结构（完整版）
typedef struct {
    __IO uint32_t CR;         // 0x00 时钟控制寄存器
    __IO uint32_t CFGR;       // 0x04 时钟配置寄存器
    __IO uint32_t CIR;        // 0x08 时钟中断寄存器
    __IO uint32_t APB2RSTR;   // 0x0C APB2外设复位寄存器
    __IO uint32_t APB1RSTR;   // 0x10 APB1外设复位寄存器
    __IO uint32_t AHBENR;     // 0x14 AHB外设时钟使能寄存器
    __IO uint32_t APB2ENR;    // 0x18 APB2外设时钟使能寄存器
    __IO uint32_t APB1ENR;    // 0x1C APB1外设时钟使能寄存器
    __IO uint32_t BDCR;       // 0x20 备份域控制寄存器
    __IO uint32_t CSR;        // 0x24 控制/状态寄存器
    __IO uint32_t AHBRSTR;    // 0x28 AHB外设复位寄存器
    __IO uint32_t CFGR2;      // 0x2C 时钟配置寄存器2
    __IO uint32_t CFGR3;      // 0x30 时钟配置寄存器3
    __IO uint32_t CR2;        // 0x34 时钟控制寄存器2
} RCC_TypeDef;

// Flash寄存器结构
typedef struct {
    __IO uint32_t ACR;        // 0x00 Flash访问控制寄存器
    __IO uint32_t KEYR;       // 0x04 Flash密钥寄存器
    __IO uint32_t OPTKEYR;    // 0x08 选项字节密钥寄存器
    __IO uint32_t SR;         // 0x0C Flash状态寄存器
    __IO uint32_t CR;         // 0x10 Flash控制寄存器
    __IO uint32_t AR;         // 0x14 Flash地址寄存器
    __IO uint32_t RESERVED;   // 0x18 保留
    __IO uint32_t OBR;        // 0x1C 选项字节寄存器
    __IO uint32_t WRPR;       // 0x20 写保护寄存器
} FLASH_TypeDef;

// 外设指针
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)GPIOD_BASE)
#define RCC     ((RCC_TypeDef *)RCC_BASE)
#define FLASH   ((FLASH_TypeDef *)FLASH_R_BASE)

// RCC时钟使能位定义
#define RCC_AHBENR_GPIOAEN   (1UL << 17)
#define RCC_AHBENR_GPIOBEN   (1UL << 18)
#define RCC_AHBENR_GPIOCEN   (1UL << 19)
#define RCC_AHBENR_GPIODEN   (1UL << 20)

// GPIO模式定义
#define GPIO_MODE_INPUT      0x00  // 输入模式
#define GPIO_MODE_OUTPUT     0x01  // 通用输出模式
#define GPIO_MODE_AF         0x02  // 复用功能模式
#define GPIO_MODE_ANALOG     0x03  // 模拟模式

// GPIO输出类型定义
#define GPIO_OTYPE_PP        0x00  // 推挽输出
#define GPIO_OTYPE_OD        0x01  // 开漏输出

// GPIO速度定义
#define GPIO_SPEED_LOW       0x00  // 低速
#define GPIO_SPEED_MEDIUM    0x01  // 中速
#define GPIO_SPEED_HIGH      0x03  // 高速

// GPIO上拉/下拉定义
#define GPIO_PUPD_NOPULL     0x00  // 无上拉/下拉
#define GPIO_PUPD_UP         0x01  // 上拉
#define GPIO_PUPD_DOWN       0x02  // 下拉

// 引脚编号
#define PIN_0   0
#define PIN_1   1
#define PIN_2   2
#define PIN_3   3
#define PIN_4   4
#define PIN_5   5
#define PIN_6   6
#define PIN_7   7

// 系统时钟频率
#define SYSTEM_CLOCK_HZ     12000000UL

// 延时函数
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        for(uint32_t j = 0; j < (SYSTEM_CLOCK_HZ/10000); j++) {
            __asm__("nop");
        }
    }
}

// 完整的GPIO初始化函数（包含所有寄存器配置）
void gpio_init_complete(GPIO_TypeDef* GPIOx, uint32_t pin, 
                        uint32_t mode, uint32_t otype, 
                        uint32_t speed, uint32_t pupd) {
    // 1. 配置模式（输入/输出/复用/模拟）
    GPIOx->MODER &= ~(0x03UL << (pin * 2));
    GPIOx->MODER |= (mode << (pin * 2));
    
    // 2. 配置输出类型（推挽/开漏）- 仅输出模式有效
    if(mode == GPIO_MODE_OUTPUT || mode == GPIO_MODE_AF) {
        GPIOx->OTYPER &= ~(0x01UL << pin);
        GPIOx->OTYPER |= (otype << pin);
    }
    
    // 3. 配置输出速度 [关键！之前缺失]
    GPIOx->OSPEEDR &= ~(0x03UL << (pin * 2));
    GPIOx->OSPEEDR |= (speed << (pin * 2));
    
    // 4. 配置上拉/下拉 [关键！之前缺失]
    GPIOx->PUPDR &= ~(0x03UL << (pin * 2));
    GPIOx->PUPDR |= (pupd << (pin * 2));
}

// 系统时钟初始化（完整版）
void system_clock_init(void) {
    // 1. 使能HSI（内部高速时钟）
    RCC->CR |= 0x01;  // HSION = 1
    
    // 2. 等待HSI就绪
    while(!(RCC->CR & 0x02));  // 等待HSIRDY = 1
    
    // 3. 配置Flash访问延迟（对于高于24MHz的时钟需要）
    FLASH->ACR = 0x01;  // 1个等待状态
    
    // 4. 选择HSI作为系统时钟
    RCC->CFGR &= ~0x03;  // SW = 00 (HSI)
    
    // 5. 等待系统时钟切换完成
    while((RCC->CFGR & 0x0C) != 0x00);  // 等待SWS = 00
    
    // 6. 配置AHB和APB时钟分频（保持默认不分频）
    RCC->CFGR &= ~0xF0;   // HPRE = 0000 (不分频)
    RCC->CFGR &= ~0x700;  // PPRE = 000 (不分频)
}

// 初始化电源控制（完整版）
void init_power_control(void) {
    // ===== PC6 - CPU电源控制（最关键！必须首先初始化） =====
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    // 使用完整的GPIO配置
    gpio_init_complete(GPIOC, PIN_6, 
        GPIO_MODE_OUTPUT,   // 输出模式
        GPIO_OTYPE_PP,      // 推挽输出
        GPIO_SPEED_HIGH,    // 高速
        GPIO_PUPD_UP);      // 上拉
    
    // 立即设置高电平锁存电源
    GPIOC->BSRR = (1UL << PIN_6);
    
    // ===== PA3 - LCD电源控制 =====
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    
    gpio_init_complete(GPIOA, PIN_3, 
        GPIO_MODE_OUTPUT, 
        GPIO_OTYPE_PP, 
        GPIO_SPEED_HIGH, 
        GPIO_PUPD_UP);
    
    GPIOA->BSRR = (1UL << PIN_3);
    
    // ===== PC5 - RF电源控制 =====
    gpio_init_complete(GPIOC, PIN_5, 
        GPIO_MODE_OUTPUT, 
        GPIO_OTYPE_PP, 
        GPIO_SPEED_HIGH, 
        GPIO_PUPD_UP);
    
    GPIOC->BSRR = (1UL << PIN_5);
}

// 初始化LED（完整版）
void init_leds(void) {
    // 确保GPIOC时钟已使能
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    // PC0 - 红色LED
    gpio_init_complete(GPIOC, PIN_0, 
        GPIO_MODE_OUTPUT, 
        GPIO_OTYPE_PP, 
        GPIO_SPEED_HIGH, 
        GPIO_PUPD_NOPULL);
    
    // PC1 - 绿色LED
    gpio_init_complete(GPIOC, PIN_1, 
        GPIO_MODE_OUTPUT, 
        GPIO_OTYPE_PP, 
        GPIO_SPEED_HIGH, 
        GPIO_PUPD_NOPULL);
    
    // 初始状态：关闭LED（高电平 = LED灭）
    GPIOC->BSRR = (1UL << PIN_0) | (1UL << PIN_1);
}

// 切换LED状态
void toggle_leds(void) {
    // 读取当前状态并翻转
    uint32_t current = GPIOC->ODR;
    
    // PC0翻转
    if(current & (1UL << PIN_0)) {
        GPIOC->BRR = (1UL << PIN_0);   // 清除位 = 低电平
    } else {
        GPIOC->BSRR = (1UL << PIN_0);  // 设置位 = 高电平
    }
    
    // PC1翻转
    if(current & (1UL << PIN_1)) {
        GPIOC->BRR = (1UL << PIN_1);
    } else {
        GPIOC->BSRR = (1UL << PIN_1);
    }
}

// 主函数
int main(void) {
    // ========== 步骤1: 系统时钟初始化（关键！） ==========
    system_clock_init();
    
    // ========== 步骤2: 紧急电源控制（必须在150ms内） ==========
    init_power_control();
    
    // 短暂延时确保电源稳定
    delay_ms(10);
    
    // ========== 步骤3: 初始化LED ==========
    init_leds();
    
    // ========== 步骤4: 快闪3次表示启动成功 ==========
    for(int i = 0; i < 3; i++) {
        toggle_leds();
        delay_ms(200);
        toggle_leds();
        delay_ms(200);
    }
    
    // ========== 步骤5: 主循环 - LED闪烁 ==========
    while(1) {
        toggle_leds();
        delay_ms(500);
    }
    
    return 0;
}

// 错误处理
void Error_Handler(void) {
    // 快速闪烁表示错误
    while(1) {
        toggle_leds();
        delay_ms(100);
    }
}

// 中断向量表已在startup_wb32l003.s中定义