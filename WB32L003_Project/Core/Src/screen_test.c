/*
 * 屏幕测试 - 尝试点亮屏幕背光
 */

#include "main.h"

int main(void) {
    // 直接寄存器操作
    // 启用所有 GPIO 时钟
    RCC->AHBENR |= (1UL << 17);  // GPIOAEN
    RCC->AHBENR |= (1UL << 18);  // GPIOBEN
    RCC->AHBENR |= (1UL << 19);  // GPIOCEN
    RCC->AHBENR |= (1UL << 20);  // GPIODEN
    
    // 延时
    for(volatile int i = 0; i < 10000; i++);
    
    // PC6 输出 LOW (保持电源)
    GPIOC->MODER &= ~(3UL << 12);
    GPIOC->MODER |= (1UL << 12);
    GPIOC->BRR = (1UL << 6);
    
    // PA3 输出 LOW (LCD 电源)
    GPIOA->MODER &= ~(3UL << 6);
    GPIOA->MODER |= (1UL << 6);
    GPIOA->BRR = (1UL << 3);
    
    // PB1 输出 LOW (背光电源)
    GPIOB->MODER &= ~(3UL << 2);
    GPIOB->MODER |= (1UL << 2);
    GPIOB->BRR = (1UL << 1);
    
    // PB2 输出 HIGH (背光亮度 - 最亮)
    GPIOB->MODER &= ~(3UL << 4);
    GPIOB->MODER |= (1UL << 4);
    GPIOB->BSRR = (1UL << 2);
    
    // LCD 数据引脚全部输出高，看是否有反应
    // PB7-PB0 都设为输出
    GPIOB->MODER &= ~0xFFFF;  // 清除 PB0-PB7 的模式位
    for(int i = 0; i < 8; i++) {
        GPIOB->MODER |= (1UL << (i * 2));  // 设为输出
        GPIOB->BSRR = (1UL << i);  // 输出高电平
    }
    
    // 尝试不同的电源组合
    uint8_t state = 0;
    
    while(1) {
        // 保持 PC6 LOW
        GPIOC->BRR = (1UL << 6);
        
        // 每秒切换一次状态
        for(volatile int i = 0; i < 1000000; i++);
        
        switch(state) {
            case 0:  // o3 逻辑
                GPIOA->BRR = (1UL << 3);   // PA3 LOW
                GPIOB->BRR = (1UL << 1);   // PB1 LOW
                GPIOB->BSRR = (1UL << 2);  // PB2 HIGH
                break;
                
            case 1:  // 洪哥逻辑
                GPIOA->BSRR = (1UL << 3);  // PA3 HIGH
                GPIOB->BSRR = (1UL << 1);  // PB1 HIGH
                GPIOB->BSRR = (1UL << 2);  // PB2 HIGH
                break;
                
            case 2:  // 混合测试
                GPIOA->BRR = (1UL << 3);   // PA3 LOW
                GPIOB->BSRR = (1UL << 1);  // PB1 HIGH
                GPIOB->BRR = (1UL << 2);   // PB2 LOW
                break;
                
            case 3:  // 脉冲 PB2 (背光控制)
                for(int i = 0; i < 10; i++) {
                    GPIOB->BSRR = (1UL << 2);  // PB2 HIGH
                    for(volatile int j = 0; j < 100000; j++);
                    GPIOB->BRR = (1UL << 2);   // PB2 LOW
                    for(volatile int j = 0; j < 100000; j++);
                }
                break;
        }
        
        state++;
        if(state > 3) state = 0;
    }
}