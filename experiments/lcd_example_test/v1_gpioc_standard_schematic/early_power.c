/*
 * 早期电源锁存 - 必须在 150ms 内执行
 */

#include <stdint.h>

// 寄存器定义
#define RCC_BASE            0x40021000UL
#define RCC_AHBENR          (*(volatile uint32_t*)(RCC_BASE + 0x14))
#define RCC_AHBENR_GPIOBEN  (1UL << 18)  // GPIOB时钟使能位

#define GPIOB_BASE          0x48000400UL
#define GPIOB_MODER         (*(volatile uint32_t*)(GPIOB_BASE + 0x00))
#define GPIOB_BRR           (*(volatile uint32_t*)(GPIOB_BASE + 0x28))

// 最早期的电源锁存函数 - 在 Reset_Handler 中调用
void Early_PowerLatch(void) {
    // 1. 开启 GPIOB 时钟（CON_POW_CPU在PB7）
    RCC_AHBENR |= RCC_AHBENR_GPIOBEN;
    
    // 2. 最小延时确保时钟稳定
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    
    // 3. PB7 设为输出
    GPIOB_MODER &= ~(3UL << (7 * 2));  // 清除 bits 14-15
    GPIOB_MODER |= (1UL << (7 * 2));   // 设置 bit 14 (输出模式)
    
    // 4. 立即拉低 PB7 - 锁住电源！
    GPIOB_BRR = (1UL << 7);
}