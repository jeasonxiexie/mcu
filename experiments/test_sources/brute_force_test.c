/*
 * 暴力测试 - 尝试所有可能的引脚组合
 */

// 不包含任何头文件，完全独立
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

// 寄存器定义
#define RCC_BASE     0x40021000UL
#define GPIOA_BASE   0x48000000UL
#define GPIOB_BASE   0x48000400UL
#define GPIOC_BASE   0x48000800UL
#define GPIOD_BASE   0x48000C00UL

#define RCC_AHBENR   (*(volatile uint32_t*)(RCC_BASE + 0x14))
#define GPIO_MODER(base)  (*(volatile uint32_t*)((base) + 0x00))
#define GPIO_ODR(base)    (*(volatile uint32_t*)((base) + 0x14))
#define GPIO_BSRR(base)   (*(volatile uint32_t*)((base) + 0x18))
#define GPIO_BRR(base)    (*(volatile uint32_t*)((base) + 0x28))

// 在 Reset_Handler 最早期调用
void Brute_Force_Power(void) {
    // 开启所有 GPIO 时钟
    RCC_AHBENR |= (1UL << 17);  // GPIOA
    RCC_AHBENR |= (1UL << 18);  // GPIOB
    RCC_AHBENR |= (1UL << 19);  // GPIOC
    RCC_AHBENR |= (1UL << 20);  // GPIOD
    
    // 延时
    for(volatile int i = 0; i < 100; i++);
    
    // 设置所有可能的电源引脚为输出
    // PC6 - 主电源？
    GPIO_MODER(GPIOC_BASE) &= ~(3UL << 12);
    GPIO_MODER(GPIOC_BASE) |= (1UL << 12);
    
    // PC5 - 5V电源？
    GPIO_MODER(GPIOC_BASE) &= ~(3UL << 10);
    GPIO_MODER(GPIOC_BASE) |= (1UL << 10);
    
    // PA3 - LCD电源？
    GPIO_MODER(GPIOA_BASE) &= ~(3UL << 6);
    GPIO_MODER(GPIOA_BASE) |= (1UL << 6);
    
    // PB1/PB2 - 背光？
    GPIO_MODER(GPIOB_BASE) &= ~(3UL << 2);
    GPIO_MODER(GPIOB_BASE) |= (1UL << 2);
    GPIO_MODER(GPIOB_BASE) &= ~(3UL << 4);
    GPIO_MODER(GPIOB_BASE) |= (1UL << 4);
    
    // PC0/PC1 - LED
    GPIO_MODER(GPIOC_BASE) &= ~(3UL << 0);
    GPIO_MODER(GPIOC_BASE) |= (1UL << 0);
    GPIO_MODER(GPIOC_BASE) &= ~(3UL << 2);
    GPIO_MODER(GPIOC_BASE) |= (1UL << 2);
    
    // 尝试所有可能的组合
    // 1. o3 的逻辑 - 所有 LOW
    GPIO_BRR(GPIOC_BASE) = (1UL << 6);  // PC6 LOW
    GPIO_BRR(GPIOC_BASE) = (1UL << 5);  // PC5 LOW
    GPIO_BRR(GPIOA_BASE) = (1UL << 3);  // PA3 LOW
    GPIO_BRR(GPIOB_BASE) = (1UL << 1);  // PB1 LOW
    GPIO_BSRR(GPIOB_BASE) = (1UL << 2); // PB2 HIGH
    
    // LED - 绿灯亮
    GPIO_BRR(GPIOC_BASE) = (1UL << 1);  // PC1 LOW = 绿灯亮
    GPIO_BSRR(GPIOC_BASE) = (1UL << 0); // PC0 HIGH = 红灯灭
}

// 替代 Early_PowerLatch
void Early_PowerLatch(void) {
    Brute_Force_Power();
}

// 主函数
int main(void) {
    // 再次确保所有引脚状态
    Brute_Force_Power();
    
    // 主循环 - 尝试不同组合
    while(1) {
        // 保持 PC6 LOW
        GPIO_BRR(GPIOC_BASE) = (1UL << 6);
        
        // 闪烁 LED
        for(volatile int i = 0; i < 500000; i++);
        GPIO_ODR(GPIOC_BASE) ^= (1UL << 0) | (1UL << 1);  // 切换两个 LED
        
        // 每10秒尝试不同的电源组合
        static uint32_t counter = 0;
        counter++;
        
        if(counter > 20) {
            counter = 0;
            // 切换 PA3
            GPIO_ODR(GPIOA_BASE) ^= (1UL << 3);
            // 切换 PC5
            GPIO_ODR(GPIOC_BASE) ^= (1UL << 5);
        }
    }
}