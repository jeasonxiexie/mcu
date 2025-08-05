/*
 * 最小电源测试 - 根据 o3 建议
 * 目的：验证板子能持续供电 & LED 可见
 */

#include <stdint.h>

// 寄存器定义
#define RCC_BASE            0x40021000UL
#define RCC_AHBENR          (*(volatile uint32_t*)(RCC_BASE + 0x14))

#define GPIOA_BASE          0x48000000UL
#define GPIOB_BASE          0x48000400UL
#define GPIOC_BASE          0x48000800UL

#define GPIO_MODER(base)    (*(volatile uint32_t*)((base) + 0x00))
#define GPIO_BSRR(base)     (*(volatile uint32_t*)((base) + 0x18))
#define GPIO_BRR(base)      (*(volatile uint32_t*)((base) + 0x28))

// 在 SystemInit 之前调用
void PowerLatch_Early(void) {
    // 1. 开启 GPIO 时钟
    RCC_AHBENR |= (1UL << 17);  // GPIOA
    RCC_AHBENR |= (1UL << 18);  // GPIOB
    RCC_AHBENR |= (1UL << 19);  // GPIOC
    
    // 2. 最小延时
    for(volatile int i = 0; i < 10; i++);
    
    // 3. 设置关键电源引脚为输出
    // PC6 - 主电源
    GPIO_MODER(GPIOC_BASE) &= ~(3UL << 12);
    GPIO_MODER(GPIOC_BASE) |= (1UL << 12);
    
    // PA3 - LCD电源
    GPIO_MODER(GPIOA_BASE) &= ~(3UL << 6);
    GPIO_MODER(GPIOA_BASE) |= (1UL << 6);
    
    // PB1 - 背光供电
    GPIO_MODER(GPIOB_BASE) &= ~(3UL << 2);
    GPIO_MODER(GPIOB_BASE) |= (1UL << 2);
    
    // PC0/PC1 - LED
    GPIO_MODER(GPIOC_BASE) &= ~(3UL << 0);
    GPIO_MODER(GPIOC_BASE) |= (1UL << 0);
    GPIO_MODER(GPIOC_BASE) &= ~(3UL << 2);
    GPIO_MODER(GPIOC_BASE) |= (1UL << 2);
    
    // 4. 立即拉低电源控制引脚
    GPIO_BRR(GPIOC_BASE) = (1UL << 6);  // PC6 LOW = 主电源 ON
    GPIO_BRR(GPIOA_BASE) = (1UL << 3);  // PA3 LOW = LCD电源 ON
    GPIO_BRR(GPIOB_BASE) = (1UL << 1);  // PB1 LOW = 背光供电 ON
    
    // 5. 初始 LED 状态 - 绿灯亮
    GPIO_BRR(GPIOC_BASE) = (1UL << 1);   // PC1 LOW = 绿灯亮
    GPIO_BSRR(GPIOC_BASE) = (1UL << 0);  // PC0 HIGH = 红灯灭
}

// Early_PowerLatch 已在 early_power.c 中定义

// 简单延时
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        for(volatile uint32_t j = 0; j < 8000; j++) {
            __asm__("nop");
        }
    }
}

int main(void) {
    // 再次确保电源引脚状态
    PowerLatch_Early();
    
    // 主循环 - 每 500ms 交替红绿灯
    uint8_t led_state = 0;
    
    while(1) {
        // 保持电源引脚低电平
        GPIO_BRR(GPIOC_BASE) = (1UL << 6);  // PC6 LOW
        GPIO_BRR(GPIOA_BASE) = (1UL << 3);  // PA3 LOW
        GPIO_BRR(GPIOB_BASE) = (1UL << 1);  // PB1 LOW
        
        // 交替闪烁红绿 LED
        if(led_state) {
            GPIO_BRR(GPIOC_BASE) = (1UL << 0);   // PC0 LOW = 红灯亮
            GPIO_BSRR(GPIOC_BASE) = (1UL << 1);  // PC1 HIGH = 绿灯灭
        } else {
            GPIO_BSRR(GPIOC_BASE) = (1UL << 0);  // PC0 HIGH = 红灯灭
            GPIO_BRR(GPIOC_BASE) = (1UL << 1);   // PC1 LOW = 绿灯亮
        }
        
        led_state = !led_state;
        delay_ms(500);  // 500ms
    }
}