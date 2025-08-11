/* QFN32 Standalone Test - 完全独立实现，无需库文件 */

/* 寄存器定义 */
#define RCC_BASE        0x40021000
#define GPIOA_BASE      0x48000000  
#define GPIOC_BASE      0x48000800
#define GPIOD_BASE      0x48000C00
#define TIM1_BASE       0x40012C00

/* RCC寄存器 */
#define RCC_AHBENR      (*(volatile unsigned int*)(RCC_BASE + 0x14))
#define RCC_APB2ENR     (*(volatile unsigned int*)(RCC_BASE + 0x18))

/* GPIO寄存器 */
#define GPIOA_MODER     (*(volatile unsigned int*)(GPIOA_BASE + 0x00))
#define GPIOA_OTYPER    (*(volatile unsigned int*)(GPIOA_BASE + 0x04))
#define GPIOA_OSPEEDR   (*(volatile unsigned int*)(GPIOA_BASE + 0x08))
#define GPIOA_PUPDR     (*(volatile unsigned int*)(GPIOA_BASE + 0x0C))
#define GPIOA_BSRR      (*(volatile unsigned int*)(GPIOA_BASE + 0x18))

#define GPIOC_MODER     (*(volatile unsigned int*)(GPIOC_BASE + 0x00))
#define GPIOC_OTYPER    (*(volatile unsigned int*)(GPIOC_BASE + 0x04))
#define GPIOC_OSPEEDR   (*(volatile unsigned int*)(GPIOC_BASE + 0x08))
#define GPIOC_PUPDR     (*(volatile unsigned int*)(GPIOC_BASE + 0x0C))
#define GPIOC_BSRR      (*(volatile unsigned int*)(GPIOC_BASE + 0x18))
#define GPIOC_AFR0      (*(volatile unsigned int*)(GPIOC_BASE + 0x20))

/* TIM1寄存器 */
#define TIM1_CR1        (*(volatile unsigned int*)(TIM1_BASE + 0x00))
#define TIM1_CCMR1      (*(volatile unsigned int*)(TIM1_BASE + 0x18))
#define TIM1_CCER       (*(volatile unsigned int*)(TIM1_BASE + 0x20))
#define TIM1_PSC        (*(volatile unsigned int*)(TIM1_BASE + 0x28))
#define TIM1_ARR        (*(volatile unsigned int*)(TIM1_BASE + 0x2C))
#define TIM1_CCR1       (*(volatile unsigned int*)(TIM1_BASE + 0x34))
#define TIM1_BDTR       (*(volatile unsigned int*)(TIM1_BASE + 0x44))

void delay(unsigned int count)
{
    while(count--);
}

void setup_pc6_pwm(void)
{
    /* 使能GPIOC时钟 */
    RCC_AHBENR |= (1 << 19);
    
    /* PC6配置为复用功能AF1 (TIM1_CH1) */
    GPIOC_MODER &= ~(0x3 << 12);
    GPIOC_MODER |= (0x2 << 12);  /* 复用模式 */
    GPIOC_AFR0 &= ~(0xF << 24);
    GPIOC_AFR0 |= (0x1 << 24);   /* AF1 */
    GPIOC_OSPEEDR |= (0x3 << 12); /* 高速 */
    GPIOC_PUPDR |= (0x1 << 12);   /* 上拉 */
    
    /* 使能TIM1时钟 */
    RCC_APB2ENR |= (1 << 11);
    
    /* 配置TIM1为100kHz PWM */
    TIM1_PSC = 11;      /* 预分频器 = 12-1 */
    TIM1_ARR = 9;       /* 自动重载 = 10-1 */
    TIM1_CCR1 = 5;      /* 占空比50% */
    
    /* PWM模式1 */
    TIM1_CCMR1 = 0x68;  /* PWM mode 1 */
    TIM1_CCER = 0x01;   /* CC1E使能 */
    TIM1_BDTR = 0x8000; /* MOE主输出使能 */
    TIM1_CR1 = 0x01;    /* 使能计数器 */
}

void setup_power_and_leds(void)
{
    /* 使能GPIOA时钟 */
    RCC_AHBENR |= (1 << 17);
    
    /* PA3输出高电平 - LCD Power */
    GPIOA_MODER &= ~(0x3 << 6);
    GPIOA_MODER |= (0x1 << 6);   /* 输出模式 */
    GPIOA_OSPEEDR |= (0x3 << 6); /* 高速 */
    GPIOA_PUPDR |= (0x1 << 6);   /* 上拉 */
    GPIOA_BSRR = (1 << 3);       /* 设置高电平 */
    
    /* PC5输出高电平 - RF Power */
    GPIOC_MODER &= ~(0x3 << 10);
    GPIOC_MODER |= (0x1 << 10);  /* 输出模式 */
    GPIOC_OSPEEDR |= (0x3 << 10); /* 高速 */
    GPIOC_PUPDR |= (0x1 << 10);  /* 上拉 */
    GPIOC_BSRR = (1 << 5);       /* 设置高电平 */
    
    /* PC0和PC1配置为LED输出 */
    GPIOC_MODER &= ~(0xF);
    GPIOC_MODER |= 0x5;          /* PC0和PC1都为输出 */
    GPIOC_OSPEEDR |= 0xF;        /* 高速 */
    GPIOC_BSRR = 0x00030000;     /* 初始关闭LED（高电平） */
}

int main(void)
{
    /* 设置PC6为100kHz PWM - 系统关键！ */
    setup_pc6_pwm();
    
    /* 设置电源和LED */
    setup_power_and_leds();
    
    /* 简单的LED闪烁 */
    while(1)
    {
        /* 红LED亮，绿LED灭 */
        GPIOC_BSRR = 0x00020001;  /* PC0低(红亮)，PC1高(绿灭) */
        delay(1000000);
        
        /* 红LED灭，绿LED亮 */
        GPIOC_BSRR = 0x00010002;  /* PC0高(红灭)，PC1低(绿亮) */
        delay(1000000);
    }
}