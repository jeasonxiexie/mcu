/**
 * @file qfn32_robust.c
 * @brief WB32L003 QFN-32封装测试程序 - 增强版
 * @note 包含完整的初始化序列，避免潜在问题
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
#define __IM volatile const
#define __OM volatile
#define __IOM volatile

// 外设基地址
#define PERIPH_BASE     0x40000000UL
#define APB1PERIPH_BASE PERIPH_BASE
#define APB2PERIPH_BASE (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE (PERIPH_BASE + 0x08000000UL)

// 系统控制空间基地址
#define SCS_BASE        (0xE000E000UL)
#define SysTick_BASE    (SCS_BASE + 0x0010UL)
#define NVIC_BASE       (SCS_BASE + 0x0100UL)
#define SCB_BASE        (SCS_BASE + 0x0D00UL)

// GPIO基地址
#define GPIOA_BASE      (AHB2PERIPH_BASE + 0x00000000UL)
#define GPIOB_BASE      (AHB2PERIPH_BASE + 0x00000400UL)
#define GPIOC_BASE      (AHB2PERIPH_BASE + 0x00000800UL)
#define GPIOD_BASE      (AHB2PERIPH_BASE + 0x00000C00UL)

// RCC基地址
#define RCC_BASE        (AHB1PERIPH_BASE + 0x00001000UL)

// UART2基地址
#define UART2_BASE      (APB1PERIPH_BASE + 0x00004400UL)

// Flash接口基地址
#define FLASH_R_BASE    (AHB1PERIPH_BASE + 0x00002000UL)

// GPIO寄存器结构 - 完整版
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

// RCC寄存器结构 - 完整版
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

// UART寄存器结构 - 完整版
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
    __IO uint32_t ACR;        // 0x00 访问控制寄存器
    __IO uint32_t KEYR;       // 0x04 密钥寄存器
    __IO uint32_t OPTKEYR;    // 0x08 选项字节密钥寄存器
    __IO uint32_t SR;         // 0x0C 状态寄存器
    __IO uint32_t CR;         // 0x10 控制寄存器
    __IO uint32_t AR;         // 0x14 地址寄存器
    __IO uint32_t RESERVED;   // 0x18 保留
    __IO uint32_t OBR;        // 0x1C 选项字节寄存器
    __IO uint32_t WRPR;       // 0x20 写保护寄存器
} FLASH_TypeDef;

// SCB寄存器结构 (系统控制块)
typedef struct {
    __IM uint32_t CPUID;      // 0x00
    __IO uint32_t ICSR;       // 0x04
    __IO uint32_t VTOR;       // 0x08
    __IO uint32_t AIRCR;      // 0x0C
    __IO uint32_t SCR;        // 0x10
    __IO uint32_t CCR;        // 0x14
    __IO uint32_t SHPR[3];    // 0x18-0x20
    __IO uint32_t SHCSR;      // 0x24
    __IO uint32_t CFSR;       // 0x28
    __IO uint32_t HFSR;       // 0x2C
    __IO uint32_t DFSR;       // 0x30
    __IO uint32_t MMFAR;      // 0x34
    __IO uint32_t BFAR;       // 0x38
    __IO uint32_t AFSR;       // 0x3C
} SCB_TypeDef;

// 外设指针
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)GPIOD_BASE)
#define RCC     ((RCC_TypeDef *)RCC_BASE)
#define UART2   ((UART_TypeDef *)UART2_BASE)
#define FLASH   ((FLASH_TypeDef *)FLASH_R_BASE)
#define SCB     ((SCB_TypeDef *)SCB_BASE)

// RCC时钟使能位
#define RCC_AHBENR_GPIOAEN   (1UL << 17)
#define RCC_AHBENR_GPIOBEN   (1UL << 18)
#define RCC_AHBENR_GPIOCEN   (1UL << 19)
#define RCC_AHBENR_GPIODEN   (1UL << 20)
#define RCC_APB1ENR_UART2EN  (1UL << 17)

// RCC时钟标志位
#define RCC_CR_HSION         (1UL << 0)
#define RCC_CR_HSIRDY        (1UL << 1)
#define RCC_CR_HSEON         (1UL << 16)
#define RCC_CR_HSERDY        (1UL << 17)
#define RCC_CR_PLLON         (1UL << 24)
#define RCC_CR_PLLRDY        (1UL << 25)

// UART状态位
#define UART_SR_PE     (1UL << 0)   // 奇偶校验错误
#define UART_SR_FE     (1UL << 1)   // 帧错误
#define UART_SR_NE     (1UL << 2)   // 噪声错误
#define UART_SR_ORE    (1UL << 3)   // 溢出错误
#define UART_SR_IDLE   (1UL << 4)   // 空闲线路检测
#define UART_SR_RXNE   (1UL << 5)   // 接收数据寄存器非空
#define UART_SR_TC     (1UL << 6)   // 发送完成
#define UART_SR_TXE    (1UL << 7)   // 发送数据寄存器空
#define UART_SR_LBD    (1UL << 8)   // LIN断开检测
#define UART_SR_CTS    (1UL << 9)   // CTS标志

// UART控制位
#define UART_CR1_SBK   (1UL << 0)   // 发送断开
#define UART_CR1_RWU   (1UL << 1)   // 接收唤醒
#define UART_CR1_RE    (1UL << 2)   // 接收使能
#define UART_CR1_TE    (1UL << 3)   // 发送使能
#define UART_CR1_IDLEIE (1UL << 4)  // IDLE中断使能
#define UART_CR1_RXNEIE (1UL << 5)  // RXNE中断使能
#define UART_CR1_TCIE  (1UL << 6)   // TC中断使能
#define UART_CR1_TXEIE (1UL << 7)   // TXE中断使能
#define UART_CR1_PEIE  (1UL << 8)   // PE中断使能
#define UART_CR1_PS    (1UL << 9)   // 奇偶校验选择
#define UART_CR1_PCE   (1UL << 10)  // 奇偶校验使能
#define UART_CR1_WAKE  (1UL << 11)  // 唤醒方法
#define UART_CR1_M     (1UL << 12)  // 字长
#define UART_CR1_UE    (1UL << 13)  // UART使能

// Flash访问控制
#define FLASH_ACR_LATENCY_0  0x00
#define FLASH_ACR_LATENCY_1  0x01
#define FLASH_ACR_PRFTBE     (1UL << 4)
#define FLASH_ACR_PRFTBS     (1UL << 5)

// 系统时钟频率
#define HSI_VALUE       8000000UL    // HSI默认频率8MHz
#define SYSTEM_CLOCK    12000000UL   // 系统时钟12MHz

// GPIO配置宏
#define GPIO_MODE_INPUT   0x00
#define GPIO_MODE_OUTPUT  0x01
#define GPIO_MODE_AF      0x02
#define GPIO_MODE_ANALOG  0x03

#define GPIO_OTYPE_PP     0x00
#define GPIO_OTYPE_OD     0x01

#define GPIO_SPEED_LOW    0x00
#define GPIO_SPEED_MEDIUM 0x01
#define GPIO_SPEED_HIGH   0x03

#define GPIO_PUPD_NOPULL  0x00
#define GPIO_PUPD_UP      0x01
#define GPIO_PUPD_DOWN    0x02

// 全局变量
static volatile uint32_t SystemCoreClock = HSI_VALUE;

// 延时函数 - 使用精确的循环计数
void delay_ms(uint32_t ms) {
    // 每毫秒的循环次数，根据系统时钟调整
    uint32_t cycles_per_ms = SystemCoreClock / 10000;
    
    for(uint32_t i = 0; i < ms; i++) {
        for(uint32_t j = 0; j < cycles_per_ms; j++) {
            __asm__ volatile("nop");
        }
    }
}

// 微秒级延时
void delay_us(uint32_t us) {
    uint32_t cycles_per_us = SystemCoreClock / 10000000;
    
    for(uint32_t i = 0; i < us; i++) {
        for(uint32_t j = 0; j < cycles_per_us; j++) {
            __asm__ volatile("nop");
        }
    }
}

// 系统时钟初始化 - 完整版
void SystemClock_Config(void) {
    // 1. 使能HSI并等待就绪
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY)) {
        // 等待HSI稳定
    }
    
    // 2. 配置Flash延迟
    // 对于12MHz，需要0等待状态；对于24MHz+需要1等待状态
    FLASH->ACR = FLASH_ACR_LATENCY_0 | FLASH_ACR_PRFTBE;
    
    // 3. 配置系统时钟源
    RCC->CFGR &= ~(0x03UL);  // 清除SW位
    RCC->CFGR |= 0x00;       // SW=00: HSI作为系统时钟
    
    // 4. 等待系统时钟切换完成
    while((RCC->CFGR & 0x0C) != 0x00) {
        // 等待SWS=00
    }
    
    // 5. 配置AHB预分频器
    RCC->CFGR &= ~(0xF0UL);  // HPRE=0000: AHB时钟不分频
    
    // 6. 配置APB预分频器
    RCC->CFGR &= ~(0x700UL); // PPRE=000: APB时钟不分频
    
    // 7. 更新系统时钟变量
    SystemCoreClock = HSI_VALUE;  // 8MHz
    
    // 8. 配置系统滴答定时器（如果需要）
    // SysTick->LOAD = (SystemCoreClock / 1000) - 1;  // 1ms中断
}

// GPIO初始化 - 完整版，包含所有必要的步骤
void GPIO_Init_Complete(GPIO_TypeDef* GPIOx, uint32_t pin, 
                        uint32_t mode, uint32_t otype, 
                        uint32_t speed, uint32_t pupd, uint32_t af) {
    // 确保参数有效
    if(pin > 15) return;
    
    // 1. 清除并设置模式
    uint32_t temp = GPIOx->MODER;
    temp &= ~(0x03UL << (pin * 2));
    temp |= (mode << (pin * 2));
    GPIOx->MODER = temp;
    
    // 2. 设置输出类型（仅对输出和AF模式有效）
    if(mode == GPIO_MODE_OUTPUT || mode == GPIO_MODE_AF) {
        temp = GPIOx->OTYPER;
        temp &= ~(0x01UL << pin);
        temp |= (otype << pin);
        GPIOx->OTYPER = temp;
        
        // 3. 设置输出速度
        temp = GPIOx->OSPEEDR;
        temp &= ~(0x03UL << (pin * 2));
        temp |= (speed << (pin * 2));
        GPIOx->OSPEEDR = temp;
    }
    
    // 4. 设置上拉/下拉
    temp = GPIOx->PUPDR;
    temp &= ~(0x03UL << (pin * 2));
    temp |= (pupd << (pin * 2));
    GPIOx->PUPDR = temp;
    
    // 5. 设置复用功能（仅对AF模式有效）
    if(mode == GPIO_MODE_AF) {
        if(pin < 8) {
            temp = GPIOx->AFR[0];
            temp &= ~(0x0FUL << (pin * 4));
            temp |= (af << (pin * 4));
            GPIOx->AFR[0] = temp;
        } else {
            temp = GPIOx->AFR[1];
            temp &= ~(0x0FUL << ((pin - 8) * 4));
            temp |= (af << ((pin - 8) * 4));
            GPIOx->AFR[1] = temp;
        }
    }
    
    // 6. 添加短延时确保配置生效
    for(volatile int i = 0; i < 10; i++) {
        __asm__ volatile("nop");
    }
}

// 电源控制初始化 - 增强版
void Power_Control_Init(void) {
    // 1. 使能所有需要的GPIO时钟
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN;
    
    // 2. 等待时钟稳定
    delay_us(10);
    
    // 3. PC6 - CPU电源控制 (Pin 23) - 最高优先级！
    GPIO_Init_Complete(GPIOC, 6, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, 
                      GPIO_SPEED_HIGH, GPIO_PUPD_UP, 0);
    GPIOC->BSRR = (1UL << 6);  // 立即设置高电平
    
    // 4. 短延时确保CPU电源稳定
    delay_us(100);
    
    // 5. PA3 - LCD电源 (Pin 7)
    GPIO_Init_Complete(GPIOA, 3, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, 
                      GPIO_SPEED_HIGH, GPIO_PUPD_UP, 0);
    GPIOA->BSRR = (1UL << 3);
    
    // 6. PC5 - RF电源 (Pin 22)
    GPIO_Init_Complete(GPIOC, 5, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, 
                      GPIO_SPEED_HIGH, GPIO_PUPD_UP, 0);
    GPIOC->BSRR = (1UL << 5);
    
    // 7. 等待所有电源稳定
    delay_ms(10);
}

// LED初始化 - 增强版
void LED_Init(void) {
    // 确保GPIOC时钟已使能
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    delay_us(10);
    
    // PC0 - 红色LED (Pin 17) - 低电平点亮
    GPIO_Init_Complete(GPIOC, 0, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, 
                      GPIO_SPEED_HIGH, GPIO_PUPD_NOPULL, 0);
    GPIOC->BSRR = (1UL << 0);  // 初始高电平(LED灭)
    
    // PC1 - 绿色LED (Pin 18) - 低电平点亮
    GPIO_Init_Complete(GPIOC, 1, GPIO_MODE_OUTPUT, GPIO_OTYPE_PP, 
                      GPIO_SPEED_HIGH, GPIO_PUPD_NOPULL, 0);
    GPIOC->BSRR = (1UL << 1);  // 初始高电平(LED灭)
}

// UART2初始化 - 增强版，包含错误处理
void UART2_Init(void) {
    // 1. 使能GPIOD和UART2时钟
    RCC->AHBENR |= RCC_AHBENR_GPIODEN;
    RCC->APB1ENR |= RCC_APB1ENR_UART2EN;
    
    // 2. 复位UART2
    RCC->APB1RSTR |= RCC_APB1ENR_UART2EN;
    delay_us(10);
    RCC->APB1RSTR &= ~RCC_APB1ENR_UART2EN;
    delay_us(10);
    
    // 3. 配置PD5为UART2_TX (AF0) - Pin 30
    GPIO_Init_Complete(GPIOD, 5, GPIO_MODE_AF, GPIO_OTYPE_PP, 
                      GPIO_SPEED_HIGH, GPIO_PUPD_UP, 0x00);
    
    // 4. 配置UART参数前先禁用UART
    UART2->CR1 &= ~UART_CR1_UE;
    
    // 5. 配置波特率 115200
    // BRR = SystemCoreClock / baud_rate
    // 对于8MHz: BRR = 8000000 / 115200 = 69
    // 对于12MHz: BRR = 12000000 / 115200 = 104
    UART2->BRR = (SystemCoreClock / 115200);
    
    // 6. 配置UART参数
    UART2->CR1 = 0;  // 清除所有设置
    UART2->CR2 = 0;  // 1个停止位
    UART2->CR3 = 0;  // 无硬件流控
    
    // 7. 配置8位数据，无校验，1停止位
    UART2->CR1 &= ~UART_CR1_M;     // 8位数据
    UART2->CR1 &= ~UART_CR1_PCE;   // 无校验
    
    // 8. 使能发送
    UART2->CR1 |= UART_CR1_TE;
    
    // 9. 使能UART
    UART2->CR1 |= UART_CR1_UE;
    
    // 10. 等待UART就绪
    delay_ms(1);
}

// UART2发送字节 - 增加超时保护
void UART2_SendByte(uint8_t data) {
    uint32_t timeout = 10000;
    
    // 等待发送缓冲区空
    while(!(UART2->SR & UART_SR_TXE) && timeout--) {
        __asm__ volatile("nop");
    }
    
    if(timeout > 0) {
        UART2->DR = data;
    }
}

// UART2发送字符串
void UART2_SendString(const char* str) {
    while(*str) {
        UART2_SendByte(*str++);
    }
}

// UART2发送数字（十进制）
void UART2_SendNumber(uint32_t num) {
    char buffer[12];
    int i = 0;
    
    if(num == 0) {
        UART2_SendByte('0');
        return;
    }
    
    while(num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    while(i > 0) {
        UART2_SendByte(buffer[--i]);
    }
}

// 系统初始化检查
uint8_t System_Check(void) {
    // 检查时钟是否正常
    if(!(RCC->CR & RCC_CR_HSIRDY)) {
        return 0;  // HSI未就绪
    }
    
    // 检查关键GPIO是否配置
    if(!(RCC->AHBENR & RCC_AHBENR_GPIOCEN)) {
        return 0;  // GPIOC时钟未使能
    }
    
    // 检查PC6是否为高电平（电源锁存）
    if(!(GPIOC->ODR & (1UL << 6))) {
        return 0;  // 电源未锁存
    }
    
    return 1;  // 系统正常
}

// 主函数
int main(void) {
    uint8_t led_state = 0;
    uint32_t tick_counter = 0;
    uint32_t loop_counter = 0;
    
    // 1. 系统时钟初始化（第一优先级）
    SystemClock_Config();
    
    // 2. 电源控制初始化（必须在150ms内完成）
    Power_Control_Init();
    
    // 3. 系统检查
    if(!System_Check()) {
        // 系统初始化失败，进入错误处理
        while(1) {
            // 什么都不做，避免损坏硬件
            __asm__ volatile("nop");
        }
    }
    
    // 4. 初始化LED
    LED_Init();
    
    // 5. 初始化UART2
    UART2_Init();
    
    // 6. 发送启动信息
    UART2_SendString("\r\n");
    UART2_SendString("=====================================\r\n");
    UART2_SendString("QFN-32 WB32L003 Robust Test v1.0\r\n");
    UART2_SendString("System Clock: ");
    UART2_SendNumber(SystemCoreClock);
    UART2_SendString(" Hz\r\n");
    UART2_SendString("=====================================\r\n");
    
    // 7. 启动信号：快闪3次
    for(int i = 0; i < 3; i++) {
        GPIOC->BRR = (1UL << 0) | (1UL << 1);  // 两个LED都亮
        delay_ms(100);
        GPIOC->BSRR = (1UL << 0) | (1UL << 1); // 两个LED都灭
        delay_ms(100);
    }
    
    UART2_SendString("System initialized successfully!\r\n");
    UART2_SendString("Starting main loop...\r\n\r\n");
    
    // 8. 主循环
    while(1) {
        // LED交替闪烁
        if(led_state == 0) {
            GPIOC->BRR = (1UL << 0);   // 红灯亮
            GPIOC->BSRR = (1UL << 1);  // 绿灯灭
            led_state = 1;
        } else {
            GPIOC->BSRR = (1UL << 0);  // 红灯灭
            GPIOC->BRR = (1UL << 1);   // 绿灯亮
            led_state = 0;
        }
        
        // 每秒输出一次调试信息
        tick_counter++;
        if(tick_counter >= 2) {  // 500ms * 2 = 1秒
            tick_counter = 0;
            loop_counter++;
            
            UART2_SendString("[");
            UART2_SendNumber(loop_counter);
            UART2_SendString("] LED: ");
            
            if(led_state) {
                UART2_SendString("RED  | ");
            } else {
                UART2_SendString("GREEN | ");
            }
            
            // 输出GPIO状态
            UART2_SendString("PC6=");
            UART2_SendNumber((GPIOC->ODR >> 6) & 1);
            UART2_SendString(" PC5=");
            UART2_SendNumber((GPIOC->ODR >> 5) & 1);
            UART2_SendString(" PA3=");
            UART2_SendNumber((GPIOA->ODR >> 3) & 1);
            UART2_SendString("\r\n");
        }
        
        delay_ms(500);
    }
    
    return 0;
}

// 错误处理 - 增强版
void Error_Handler(void) {
    // 禁用中断
    __asm__ volatile("cpsid i");
    
    // 尝试发送错误信息
    if(UART2->CR1 & UART_CR1_UE) {
        UART2_SendString("\r\n!!! SYSTEM ERROR !!!\r\n");
    }
    
    // 错误指示：快速闪烁
    while(1) {
        if(RCC->AHBENR & RCC_AHBENR_GPIOCEN) {
            GPIOC->BRR = (1UL << 0) | (1UL << 1);
            delay_ms(50);
            GPIOC->BSRR = (1UL << 0) | (1UL << 1);
            delay_ms(50);
        } else {
            __asm__ volatile("nop");
        }
    }
}

// HardFault处理
void HardFault_Handler(void) {
    Error_Handler();
}

// NMI处理
void NMI_Handler(void) {
    Error_Handler();
}