/**
 * @file test_hal.c
 * @brief WB32L003 测试程序 - 使用HAL库方式
 * @note 基于TIM10_Count成功案例的初始化方式
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
#define GPIOB_BASE      (AHB2PERIPH_BASE + 0x00000400UL)
#define GPIOC_BASE      (AHB2PERIPH_BASE + 0x00000800UL)
#define GPIOD_BASE      (AHB2PERIPH_BASE + 0x00000C00UL)
#define GPIOF_BASE      (AHB2PERIPH_BASE + 0x00001400UL)

// RCC基地址
#define RCC_BASE        (AHB1PERIPH_BASE + 0x00001000UL)

// TIM1基地址
#define TIM1_BASE       (APB2PERIPH_BASE + 0x00002C00UL)

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

// TIM1寄存器结构
typedef struct {
    __IO uint32_t CR1;        // 0x00
    __IO uint32_t CR2;        // 0x04
    __IO uint32_t SMCR;       // 0x08
    __IO uint32_t DIER;       // 0x0C
    __IO uint32_t SR;         // 0x10
    __IO uint32_t EGR;        // 0x14
    __IO uint32_t CCMR1;      // 0x18
    __IO uint32_t CCMR2;      // 0x1C
    __IO uint32_t CCER;       // 0x20
    __IO uint32_t CNT;        // 0x24
    __IO uint32_t PSC;        // 0x28
    __IO uint32_t ARR;        // 0x2C
    __IO uint32_t RCR;        // 0x30
    __IO uint32_t CCR1;       // 0x34
    __IO uint32_t CCR2;       // 0x38
    __IO uint32_t CCR3;       // 0x3C
    __IO uint32_t CCR4;       // 0x40
    __IO uint32_t BDTR;       // 0x44
    __IO uint32_t DCR;        // 0x48
    __IO uint32_t DMAR;       // 0x4C
} TIM_TypeDef;

// 外设指针
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOF   ((GPIO_TypeDef *)GPIOF_BASE)
#define RCC     ((RCC_TypeDef *)RCC_BASE)
#define TIM1    ((TIM_TypeDef *)TIM1_BASE)

// GPIO引脚定义
#define GPIO_PIN_0    0x0001
#define GPIO_PIN_1    0x0002
#define GPIO_PIN_2    0x0004
#define GPIO_PIN_3    0x0008
#define GPIO_PIN_4    0x0010
#define GPIO_PIN_5    0x0020
#define GPIO_PIN_6    0x0040
#define GPIO_PIN_7    0x0080
#define GPIO_PIN_8    0x0100
#define GPIO_PIN_9    0x0200
#define GPIO_PIN_10   0x0400
#define GPIO_PIN_11   0x0800
#define GPIO_PIN_12   0x1000
#define GPIO_PIN_13   0x2000
#define GPIO_PIN_14   0x4000
#define GPIO_PIN_15   0x8000

// RCC时钟使能位
#define RCC_AHBENR_GPIOAEN   (1UL << 17)
#define RCC_AHBENR_GPIOBEN   (1UL << 18)
#define RCC_AHBENR_GPIOCEN   (1UL << 19)
#define RCC_AHBENR_GPIODEN   (1UL << 20)
#define RCC_AHBENR_GPIOFEN   (1UL << 22)
#define RCC_APB2ENR_TIM1EN   (1UL << 11)

// GPIO配置标志（模拟TIM10_Count的GPIO_Init参数）
#define GPIO_MODE_OUTPUT    0x01
#define GPIO_MODE_AF        0x02
#define GPIO_OTYPE_PP       0x00
#define GPIO_OTYPE_OD       0x01
#define GPIO_SPEED_LOW      0x00
#define GPIO_SPEED_MEDIUM   0x01
#define GPIO_SPEED_HIGH     0x03
#define GPIO_PUPD_NOPULL    0x00
#define GPIO_PUPD_UP        0x01
#define GPIO_PUPD_DOWN      0x02
#define GPIO_AF0            0x00
#define GPIO_AF1            0x01

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

// GPIO初始化函数（模拟HAL库的GPIO_Init）
void GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t config) {
    uint32_t pin_pos = 0;
    uint32_t pin_mask = GPIO_Pin;
    
    // 解析配置
    uint32_t mode = (config >> 0) & 0x03;
    uint32_t otype = (config >> 4) & 0x01;
    uint32_t speed = (config >> 8) & 0x03;
    uint32_t pupd = (config >> 12) & 0x03;
    uint32_t af = (config >> 16) & 0x0F;
    
    // 配置每个引脚
    while(pin_mask != 0) {
        if(pin_mask & 0x01) {
            // MODER - 模式配置
            GPIOx->MODER &= ~(0x03UL << (pin_pos * 2));
            GPIOx->MODER |= (mode << (pin_pos * 2));
            
            // OTYPER - 输出类型
            if(mode == GPIO_MODE_OUTPUT || mode == GPIO_MODE_AF) {
                GPIOx->OTYPER &= ~(0x01UL << pin_pos);
                GPIOx->OTYPER |= (otype << pin_pos);
            }
            
            // OSPEEDR - 输出速度
            GPIOx->OSPEEDR &= ~(0x03UL << (pin_pos * 2));
            GPIOx->OSPEEDR |= (speed << (pin_pos * 2));
            
            // PUPDR - 上拉/下拉
            GPIOx->PUPDR &= ~(0x03UL << (pin_pos * 2));
            GPIOx->PUPDR |= (pupd << (pin_pos * 2));
            
            // AFR - 复用功能
            if(mode == GPIO_MODE_AF) {
                if(pin_pos < 8) {
                    GPIOx->AFR[0] &= ~(0x0FUL << (pin_pos * 4));
                    GPIOx->AFR[0] |= (af << (pin_pos * 4));
                } else {
                    GPIOx->AFR[1] &= ~(0x0FUL << ((pin_pos - 8) * 4));
                    GPIOx->AFR[1] |= (af << ((pin_pos - 8) * 4));
                }
            }
        }
        pin_mask >>= 1;
        pin_pos++;
    }
}

// 创建配置字
#define GPIO_CONFIG(mode, otype, speed, pupd, af) \
    ((mode) | ((otype) << 4) | ((speed) << 8) | ((pupd) << 12) | ((af) << 16))

// TIM1配置（基于TIM10_Count的Tim1_Config）
void Tim1_Config(void) {
    // 使能GPIOC时钟
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    // 配置PC6为TIM1_CH1（复用功能AF1）
    // 对应TIM10_Count中的：GPIO_PUPD_UP| GPIO_OTYPE_PP| GPIO_SPEED_HIGH| GPIO_AF1
    GPIO_Init(GPIOC, GPIO_PIN_6, 
        GPIO_CONFIG(GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_HIGH, GPIO_PUPD_UP, GPIO_AF1));
    
    // 使能TIM1时钟
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    
    // 配置TIM1为100kHz PWM
    // 参考TIM10_Count: Period = 10-1, Prescaler = 12-1
    TIM1->PSC = 12 - 1;        // 预分频器
    TIM1->ARR = 10 - 1;         // 自动重装载值
    TIM1->CCR1 = 5;             // 占空比50%
    
    // 配置为PWM模式1
    TIM1->CCMR1 &= ~0x7F;       // 清除OC1M, CC1S
    TIM1->CCMR1 |= 0x60;        // OC1M = 110 (PWM模式1)
    TIM1->CCMR1 |= 0x08;        // OC1PE = 1 (预装载使能)
    
    // 使能通道1输出
    TIM1->CCER |= 0x01;         // CC1E = 1
    
    // 使能主输出
    TIM1->BDTR |= 0x8000;       // MOE = 1
    
    // 使能TIM1
    TIM1->CR1 |= 0x01;          // CEN = 1
}

// 初始化电源控制引脚
void init_power_control(void) {
    // PC6 - CPU电源（必须首先初始化）
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIO_Init(GPIOC, GPIO_PIN_6, 
        GPIO_CONFIG(GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO_SPEED_HIGH, GPIO_PUPD_UP, 0));
    GPIOC->BSRR = GPIO_PIN_6;  // 设置高电平
    
    // PA3 - LCD电源
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIO_Init(GPIOA, GPIO_PIN_3, 
        GPIO_CONFIG(GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO_SPEED_HIGH, GPIO_PUPD_UP, 0));
    GPIOA->BSRR = GPIO_PIN_3;  // 设置高电平
    
    // PC5 - RF电源
    GPIO_Init(GPIOC, GPIO_PIN_5, 
        GPIO_CONFIG(GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO_SPEED_HIGH, GPIO_PUPD_UP, 0));
    GPIOC->BSRR = GPIO_PIN_5;  // 设置高电平
}

// 初始化LED
void init_leds(void) {
    // PC0 - 红色LED
    // PC1 - 绿色LED
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    GPIO_Init(GPIOC, GPIO_PIN_0 | GPIO_PIN_1, 
        GPIO_CONFIG(GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, GPIO_SPEED_HIGH, GPIO_PUPD_NOPULL, 0));
    
    // 初始状态：关闭LED（假设低电平有效）
    GPIOC->BSRR = (GPIO_PIN_0 | GPIO_PIN_1);  // 设置高电平 = LED灭
}

// 切换LED
void toggle_leds(void) {
    // 读取当前状态并翻转
    if(GPIOC->ODR & GPIO_PIN_0) {
        GPIOC->BRR = GPIO_PIN_0;  // 清除位 = 低电平
    } else {
        GPIOC->BSRR = GPIO_PIN_0; // 设置位 = 高电平
    }
    
    if(GPIOC->ODR & GPIO_PIN_1) {
        GPIOC->BRR = GPIO_PIN_1;
    } else {
        GPIOC->BSRR = GPIO_PIN_1;
    }
}

// 系统时钟配置
void SystemClock_Config(void) {
    // 使能HSI
    RCC->CR |= 0x01;  // HSION = 1
    
    // 等待HSI就绪
    while(!(RCC->CR & 0x02));  // 等待HSIRDY
    
    // 配置Flash延迟
    *((volatile uint32_t*)0x40022000) = 0x01;  // FLASH_ACR = 1等待状态
    
    // 配置PLL（如果需要）
    // 这里保持默认HSI 8MHz
}

// 主函数
int main(void) {
    // 系统初始化
    SystemClock_Config();
    
    // 紧急：电源控制（必须在150ms内）
    init_power_control();
    
    // 可选：使用TIM1方式初始化PC6（类似TIM10_Count）
    // Tim1_Config();
    
    // 初始化LED
    init_leds();
    
    // 快闪3次表示启动成功
    for(int i = 0; i < 3; i++) {
        toggle_leds();
        delay_ms(200);
        toggle_leds();
        delay_ms(200);
    }
    
    // 主循环
    while(1) {
        toggle_leds();
        delay_ms(500);
    }
    
    return 0;
}

// 错误处理
void Error_Handler(void) {
    while(1) {
        toggle_leds();
        delay_ms(100);
    }
}