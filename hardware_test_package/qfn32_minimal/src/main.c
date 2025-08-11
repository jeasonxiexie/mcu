/**
 * @file main.c
 * @brief WB32L003 QFN32最小测试程序
 * @note LED闪烁 + 软件串口调试输出
 */

// 基本类型定义
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
#include "../inc/board_pins.h"
#include "../inc/wb32_hal_like.h"

// SysTick 1μs时基
static volatile uint32_t usTicks = 0;

// SysTick中断处理（每1μs调用）
void SysTick_Handler(void) { 
    usTicks++; 
}

// 微秒延时
static inline void delay_us(uint32_t us) { 
    uint32_t t = usTicks + us; 
    while((int32_t)(t - usTicks) > 0) {
        __NOP();
    }
}

// 毫秒延时
static inline void delay_ms(uint32_t ms) { 
    while(ms--) {
        delay_us(1000);
    }
}

// GPIO配置为推挽输出（完整配置）
static inline void gpio_out_pp(volatile GPIO_TypeDef* g, uint8_t pin) {
    // MODER: 01 = 输出模式
    g->MODER = (g->MODER & ~(3u << (pin * 2))) | (1u << (pin * 2));
    // OTYPER: 0 = 推挽输出
    g->OTYPER &= ~(1u << pin);
    // OSPEEDR: 00 = 低速（省电）
    g->OSPEEDR &= ~(3u << (pin * 2));
    // PUPDR: 00 = 无上下拉
    g->PUPDR &= ~(3u << (pin * 2));
}

// 引脚拉高（使用BSRR低16位）
static inline void pin_hi(volatile GPIO_TypeDef* g, uint8_t pin) { 
    g->BSRR = (1u << pin); 
}

// 引脚拉低（使用BSRR高16位）
static inline void pin_lo(volatile GPIO_TypeDef* g, uint8_t pin) { 
    g->BSRR = (1u << (pin + 16)); 
}

// 软件串口初始化
static inline void softuart_init(void) {
    gpio_out_pp(DBG_TX_PORT, DBG_TX_PIN);
    pin_hi(DBG_TX_PORT, DBG_TX_PIN); // 空闲状态为高
}

// 软件串口发送单字节
static inline void softuart_tx_byte(uint8_t b) {
    // 起始位（低电平）
    pin_lo(DBG_TX_PORT, DBG_TX_PIN); 
    delay_us(BIT_US);
    
    // 8个数据位（LSB first）
    for(int i = 0; i < 8; i++) {
        if(b & 1) {
            pin_hi(DBG_TX_PORT, DBG_TX_PIN); 
        } else {
            pin_lo(DBG_TX_PORT, DBG_TX_PIN);
        }
        delay_us(BIT_US); 
        b >>= 1;
    }
    
    // 停止位（高电平）
    pin_hi(DBG_TX_PORT, DBG_TX_PIN); 
    delay_us(BIT_US);
}

// 软件串口发送字符串
static inline void softuart_tx_str(const char* s) { 
    while(*s) {
        softuart_tx_byte((uint8_t)*s++);
    }
}

// 主函数
int main(void) {
    // 1. 系统时钟配置（假设已是32MHz）
    SystemClock_Config();
    
    // 2. 配置SysTick为1μs时基
    // SysTick->LOAD = SystemCoreClock/1000000 - 1
    // 对于32MHz: 32000000/1000000 = 32
    *((volatile uint32_t*)0xE000E014) = 31;  // SysTick->LOAD
    *((volatile uint32_t*)0xE000E018) = 0;   // SysTick->VAL
    *((volatile uint32_t*)0xE000E010) = 7;   // SysTick->CTRL = ENABLE | TICKINT | CLKSOURCE

    // 3. 使能所有GPIO时钟
    RCC_EnableGPIOA(); 
    RCC_EnableGPIOB(); 
    RCC_EnableGPIOC(); 
    RCC_EnableGPIOD();
    
    // 短延时确保时钟稳定
    for(int i = 0; i < 100; i++) __NOP();

    // 4. 电源门控（三路必须全部拉高）
    gpio_out_pp(POW_CPU_PORT, POW_CPU_PIN);
    gpio_out_pp(POW_LCD_PORT, POW_LCD_PIN);
    gpio_out_pp(POW_RF_PORT,  POW_RF_PIN);
    
    pin_hi(POW_CPU_PORT, POW_CPU_PIN);  // PC6 = HIGH
    pin_hi(POW_LCD_PORT, POW_LCD_PIN);  // PA3 = HIGH
    pin_hi(POW_RF_PORT,  POW_RF_PIN);   // PC5 = HIGH
    
    // 等待电源稳定
    delay_ms(10);

    // 5. LED初始化（低电平点亮，初始熄灭）
    gpio_out_pp(LED_R_PORT, LED_R_PIN);
    gpio_out_pp(LED_G_PORT, LED_G_PIN);
    pin_hi(LED_R_PORT, LED_R_PIN);  // 红灯熄灭
    pin_hi(LED_G_PORT, LED_G_PIN);  // 绿灯熄灭

    // 6. 软件串口初始化
    softuart_init();
    softuart_tx_str("\r\n[BOOT] WB32L003 ready @38400 8N1\r\n");
    softuart_tx_str("[INFO] Power: PC6=CPU PA3=LCD PC5=RF\r\n");
    softuart_tx_str("[INFO] LEDs: PC0=RED PC1=GREEN\r\n");
    softuart_tx_str("[INFO] UART: PB4 Software TX\r\n");

    // 7. 主循环 - LED交替闪烁
    uint32_t tick_counter = 0;
    
    while(1) {
        // 红亮绿灭（250ms）
        pin_lo(LED_R_PORT, LED_R_PIN);  // 红灯亮
        pin_hi(LED_G_PORT, LED_G_PIN);  // 绿灯灭
        delay_ms(250);
        
        // 红灭绿亮（250ms）
        pin_hi(LED_R_PORT, LED_R_PIN);  // 红灯灭
        pin_lo(LED_G_PORT, LED_G_PIN);  // 绿灯亮
        delay_ms(250);
        
        // 每秒输出一次心跳
        if(++tick_counter % 2 == 0) {
            softuart_tx_str("[tick] 1s\r\n");
        }
    }
    
    return 0;
}

// 硬件错误处理
void HardFault_Handler(void) {
    // 快速闪烁表示错误
    while(1) {
        pin_lo(LED_R_PORT, LED_R_PIN);
        delay_ms(50);
        pin_hi(LED_R_PORT, LED_R_PIN);
        delay_ms(50);
    }
}

// 默认中断处理 - 在startup文件中已定义
// void Default_Handler(void) {
//     while(1) {
//         __NOP();
//     }
// }