/*
 * 早期电源锁存 - 必须在 150ms 内执行
 */

#include <stdint.h>

// 寄存器定义（根据 docs：PC6 = CON_POW_CPU，HIGH=开/保持）
#define RCC_BASE             0x40021000UL
#define RCC_AHBENR           (*(volatile uint32_t*)(RCC_BASE + 0x14))
#define RCC_AHBENR_GPIOCEN   (1UL << 19)  // GPIOC时钟使能位

#define GPIOC_BASE           0x48000800UL
#define GPIOC_MODER          (*(volatile uint32_t*)(GPIOC_BASE + 0x00))
#define GPIOC_BSRR           (*(volatile uint32_t*)(GPIOC_BASE + 0x18))

// 最早期的电源锁存函数 - 在 Reset_Handler 中调用
void Early_PowerLatch(void) {
    // 1. 开启 GPIOC 时钟（CON_POW_CPU 在 PC6，HIGH = 开/保持）
    RCC_AHBENR |= RCC_AHBENR_GPIOCEN;

    // 2. 最小延时确保时钟稳定
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");

    // 3. PC6 设为输出
    GPIOC_MODER &= ~(3UL << (6 * 2));  // 清除 bits 12-13
    GPIOC_MODER |=  (1UL << (6 * 2));  // 设置 bit 12 (输出模式)

    // 4. 立即拉高 PC6 - 锁住电源！
    GPIOC_BSRR = (1UL << 6);
}