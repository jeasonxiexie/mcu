/**
 * @file simple_test.c
 * @brief WB32L003 硬件测试程序 - 可配置版本
 * @author BensProject Team
 * @date 2024
 * 
 * 说明：这是一个最简化的测试程序，用于验证MCU基本功能
 * 主要功能：
 * 1. 电源锁存
 * 2. LED闪烁
 * 3. 串口输出（可选）
 */

#include <stdint.h>

// ============================================================
// 配置区域 - 根据实际硬件修改这里！
// ============================================================

// LED配置
#define LED1_GPIO           'C'      // LED1所在GPIO组 ('A', 'B', 'C', 'D')
#define LED1_PIN            0         // LED1引脚号 (0-15)
#define LED2_GPIO           'C'      // LED2所在GPIO组
#define LED2_PIN            1         // LED2引脚号

// LED极性配置
#define LED_ACTIVE_LOW      1         // 1=低电平亮, 0=高电平亮

// 电源锁存配置（根据RTF文档，这个应该是正确的）
#define POWER_LATCH_GPIO    'C'      // 电源锁存GPIO组
#define POWER_LATCH_PIN     6         // 电源锁存引脚 (PC6)

// 串口配置（可选）
#define ENABLE_UART         1         // 1=启用串口, 0=禁用串口
#define UART_TX_GPIO        'A'      // UART TX GPIO组
#define UART_TX_PIN         9         // UART TX引脚 (PA9)

// ============================================================
// 寄存器定义 - 不要修改
// ============================================================

// RCC寄存器
#define RCC_BASE            0x40021000UL
#define RCC_AHBENR          (*(volatile uint32_t*)(RCC_BASE + 0x14))
#define RCC_APB2ENR         (*(volatile uint32_t*)(RCC_BASE + 0x18))

// GPIO寄存器基地址
#define GPIOA_BASE          0x48000000UL
#define GPIOB_BASE          0x48000400UL
#define GPIOC_BASE          0x48000800UL
#define GPIOD_BASE          0x48000C00UL

// GPIO寄存器偏移
#define GPIO_MODER_OFFSET   0x00
#define GPIO_OTYPER_OFFSET  0x04
#define GPIO_OSPEEDR_OFFSET 0x08
#define GPIO_PUPDR_OFFSET   0x0C
#define GPIO_IDR_OFFSET     0x10
#define GPIO_ODR_OFFSET     0x14
#define GPIO_BSRR_OFFSET    0x18
#define GPIO_BRR_OFFSET     0x28
#define GPIO_AFR_OFFSET     0x20

// USART1寄存器
#define USART1_BASE         0x40013800UL
#define USART1_CR1          (*(volatile uint32_t*)(USART1_BASE + 0x00))
#define USART1_BRR          (*(volatile uint32_t*)(USART1_BASE + 0x0C))
#define USART1_ISR          (*(volatile uint32_t*)(USART1_BASE + 0x1C))
#define USART1_TDR          (*(volatile uint32_t*)(USART1_BASE + 0x28))

// RCC时钟使能位
#define RCC_AHBENR_GPIOAEN  (1UL << 17)
#define RCC_AHBENR_GPIOBEN  (1UL << 18)
#define RCC_AHBENR_GPIOCEN  (1UL << 19)
#define RCC_AHBENR_GPIODEN  (1UL << 20)
#define RCC_APB2ENR_USART1EN (1UL << 14)

// USART控制位
#define USART_CR1_UE        (1UL << 0)
#define USART_CR1_TE        (1UL << 3)
#define USART_ISR_TXE       (1UL << 7)

// ============================================================
// 辅助函数
// ============================================================

// 获取GPIO基地址
uint32_t get_gpio_base(char gpio) {
    switch(gpio) {
        case 'A': case 'a': return GPIOA_BASE;
        case 'B': case 'b': return GPIOB_BASE;
        case 'C': case 'c': return GPIOC_BASE;
        case 'D': case 'd': return GPIOD_BASE;
        default: return 0;
    }
}

// 使能GPIO时钟
void enable_gpio_clock(char gpio) {
    switch(gpio) {
        case 'A': case 'a': RCC_AHBENR |= RCC_AHBENR_GPIOAEN; break;
        case 'B': case 'b': RCC_AHBENR |= RCC_AHBENR_GPIOBEN; break;
        case 'C': case 'c': RCC_AHBENR |= RCC_AHBENR_GPIOCEN; break;
        case 'D': case 'd': RCC_AHBENR |= RCC_AHBENR_GPIODEN; break;
    }
}

// 配置GPIO为输出
void gpio_set_output(char gpio, uint8_t pin) {
    uint32_t base = get_gpio_base(gpio);
    if (base == 0) return;
    
    volatile uint32_t* moder = (volatile uint32_t*)(base + GPIO_MODER_OFFSET);
    *moder &= ~(3UL << (pin * 2));  // 清除原有模式
    *moder |= (1UL << (pin * 2));   // 设置为输出模式
}

// 设置GPIO输出高电平
void gpio_set_high(char gpio, uint8_t pin) {
    uint32_t base = get_gpio_base(gpio);
    if (base == 0) return;
    
    volatile uint32_t* bsrr = (volatile uint32_t*)(base + GPIO_BSRR_OFFSET);
    *bsrr = (1UL << pin);
}

// 设置GPIO输出低电平
void gpio_set_low(char gpio, uint8_t pin) {
    uint32_t base = get_gpio_base(gpio);
    if (base == 0) return;
    
    volatile uint32_t* brr = (volatile uint32_t*)(base + GPIO_BRR_OFFSET);
    *brr = (1UL << pin);
}

// 切换GPIO输出电平
void gpio_toggle(char gpio, uint8_t pin) {
    uint32_t base = get_gpio_base(gpio);
    if (base == 0) return;
    
    volatile uint32_t* odr = (volatile uint32_t*)(base + GPIO_ODR_OFFSET);
    *odr ^= (1UL << pin);
}

// 延时函数（粗略）
void delay_ms(uint32_t ms) {
    // 假设32MHz时钟，这个值需要根据实际调整
    for(uint32_t i = 0; i < ms; i++) {
        for(uint32_t j = 0; j < 4000; j++) {
            __asm__("nop");
        }
    }
}

// ============================================================
// 串口函数（可选）
// ============================================================

#if ENABLE_UART

void uart_init(void) {
    // 使能GPIOA和USART1时钟
    enable_gpio_clock(UART_TX_GPIO);
    RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
    
    // 配置TX引脚为复用功能
    uint32_t gpio_base = get_gpio_base(UART_TX_GPIO);
    volatile uint32_t* moder = (volatile uint32_t*)(gpio_base + GPIO_MODER_OFFSET);
    volatile uint32_t* afr = (volatile uint32_t*)(gpio_base + GPIO_AFR_OFFSET + ((UART_TX_PIN / 8) * 4));
    
    *moder &= ~(3UL << (UART_TX_PIN * 2));
    *moder |= (2UL << (UART_TX_PIN * 2));  // 复用模式
    
    uint8_t afr_pos = (UART_TX_PIN % 8) * 4;
    *afr &= ~(0xFUL << afr_pos);
    *afr |= (1UL << afr_pos);  // AF1 for USART1
    
    // 配置USART1: 115200, 8N1
    USART1_BRR = 17;  // 32MHz / (16 * 115200) ≈ 17
    USART1_CR1 = USART_CR1_TE | USART_CR1_UE;
}

void uart_send_char(char c) {
    while(!(USART1_ISR & USART_ISR_TXE));
    USART1_TDR = c;
}

void uart_send_string(const char* str) {
    while(*str) {
        uart_send_char(*str++);
    }
}

#endif // ENABLE_UART

// ============================================================
// 主函数
// ============================================================

int main(void) {
    // ========== 第1步：电源锁存（最重要！必须在150ms内执行） ==========
    enable_gpio_clock(POWER_LATCH_GPIO);
    gpio_set_output(POWER_LATCH_GPIO, POWER_LATCH_PIN);
    gpio_set_high(POWER_LATCH_GPIO, POWER_LATCH_PIN);  // 拉高锁存电源
    
    // ========== 第2步：初始化LED ==========
    enable_gpio_clock(LED1_GPIO);
    enable_gpio_clock(LED2_GPIO);
    gpio_set_output(LED1_GPIO, LED1_PIN);
    gpio_set_output(LED2_GPIO, LED2_PIN);
    
    // 初始状态：关闭LED
    #if LED_ACTIVE_LOW
        gpio_set_high(LED1_GPIO, LED1_PIN);  // 高电平=灭
        gpio_set_high(LED2_GPIO, LED2_PIN);
    #else
        gpio_set_low(LED1_GPIO, LED1_PIN);   // 低电平=灭
        gpio_set_low(LED2_GPIO, LED2_PIN);
    #endif
    
    // ========== 第3步：初始化串口（可选） ==========
    #if ENABLE_UART
        uart_init();
        uart_send_string("\r\n==== WB32L003 Hardware Test ====\r\n");
        uart_send_string("Power latched on P");
        uart_send_char(POWER_LATCH_GPIO);
        uart_send_char('0' + POWER_LATCH_PIN);
        uart_send_string("\r\n");
        uart_send_string("LED1: P");
        uart_send_char(LED1_GPIO);
        uart_send_char('0' + LED1_PIN);
        uart_send_string("\r\n");
        uart_send_string("LED2: P");
        uart_send_char(LED2_GPIO);
        uart_send_char('0' + LED2_PIN);
        uart_send_string("\r\n");
        uart_send_string("Starting LED flash test...\r\n");
    #endif
    
    // ========== 第4步：启动闪烁3次表示成功 ==========
    for(int i = 0; i < 3; i++) {
        #if LED_ACTIVE_LOW
            gpio_set_low(LED1_GPIO, LED1_PIN);   // 亮
            gpio_set_low(LED2_GPIO, LED2_PIN);
        #else
            gpio_set_high(LED1_GPIO, LED1_PIN);  // 亮
            gpio_set_high(LED2_GPIO, LED2_PIN);
        #endif
        
        delay_ms(200);
        
        #if LED_ACTIVE_LOW
            gpio_set_high(LED1_GPIO, LED1_PIN);  // 灭
            gpio_set_high(LED2_GPIO, LED2_PIN);
        #else
            gpio_set_low(LED1_GPIO, LED1_PIN);   // 灭
            gpio_set_low(LED2_GPIO, LED2_PIN);
        #endif
        
        delay_ms(200);
    }
    
    // ========== 第5步：主循环 - 交替闪烁 ==========
    uint32_t counter = 0;
    while(1) {
        // LED1亮，LED2灭
        #if LED_ACTIVE_LOW
            gpio_set_low(LED1_GPIO, LED1_PIN);   // LED1亮
            gpio_set_high(LED2_GPIO, LED2_PIN);  // LED2灭
        #else
            gpio_set_high(LED1_GPIO, LED1_PIN);  // LED1亮
            gpio_set_low(LED2_GPIO, LED2_PIN);   // LED2灭
        #endif
        
        delay_ms(500);
        
        // LED1灭，LED2亮
        #if LED_ACTIVE_LOW
            gpio_set_high(LED1_GPIO, LED1_PIN);  // LED1灭
            gpio_set_low(LED2_GPIO, LED2_PIN);   // LED2亮
        #else
            gpio_set_low(LED1_GPIO, LED1_PIN);   // LED1灭
            gpio_set_high(LED2_GPIO, LED2_PIN);  // LED2亮
        #endif
        
        delay_ms(500);
        
        #if ENABLE_UART
            if ((counter & 0x07) == 0) {  // 每8次循环输出一次
                uart_send_string("[ALIVE] ");
                // 简单输出16进制计数，避免除法
                uart_send_string("0x");
                uint8_t hex = ((counter >> 12) & 0xF);
                uart_send_char(hex < 10 ? '0' + hex : 'A' + hex - 10);
                hex = ((counter >> 8) & 0xF);
                uart_send_char(hex < 10 ? '0' + hex : 'A' + hex - 10);
                hex = ((counter >> 4) & 0xF);
                uart_send_char(hex < 10 ? '0' + hex : 'A' + hex - 10);
                hex = (counter & 0xF);
                uart_send_char(hex < 10 ? '0' + hex : 'A' + hex - 10);
                uart_send_string("\r\n");
            }
        #endif
        
        counter++;
    }
}

// 简单的错误处理
void HardFault_Handler(void) {
    // 如果发生硬件错误，快速闪烁
    while(1) {
        gpio_toggle(LED1_GPIO, LED1_PIN);
        gpio_toggle(LED2_GPIO, LED2_PIN);
        delay_ms(100);
    }
}