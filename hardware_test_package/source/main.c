/**
 * @file main.c
 * @brief WB32L003 极简测试程序 - 只测试电源锁存和LED
 * @note 直接操作寄存器，不依赖HAL库
 */

#include "config.h"

// 基本类型定义
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

// 寄存器结构体定义
typedef struct {
    volatile uint32_t MODER;    // 0x00
    volatile uint32_t OTYPER;   // 0x04
    volatile uint32_t OSPEEDR;  // 0x08
    volatile uint32_t PUPDR;    // 0x0C
    volatile uint32_t IDR;      // 0x10
    volatile uint32_t ODR;      // 0x14
    volatile uint32_t BSRR;     // 0x18
    volatile uint32_t LCKR;     // 0x1C
    volatile uint32_t AFR[2];   // 0x20-0x24
    volatile uint32_t BRR;      // 0x28
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR;       // 0x00
    volatile uint32_t CFGR;     // 0x04
    volatile uint32_t CIR;      // 0x08
    volatile uint32_t APB2RSTR; // 0x0C
    volatile uint32_t APB1RSTR; // 0x10
    volatile uint32_t AHBENR;   // 0x14
    volatile uint32_t APB2ENR;  // 0x18
    volatile uint32_t APB1ENR;  // 0x1C
} RCC_TypeDef;

#if UART_ENABLE
typedef struct {
    volatile uint32_t CR1;      // 0x00
    volatile uint32_t CR2;      // 0x04
    volatile uint32_t CR3;      // 0x08
    volatile uint32_t BRR;      // 0x0C
    volatile uint32_t GTPR;     // 0x10
    volatile uint32_t RTOR;     // 0x14
    volatile uint32_t RQR;      // 0x18
    volatile uint32_t ISR;      // 0x1C
    volatile uint32_t ICR;      // 0x20
    volatile uint32_t RDR;      // 0x24
    volatile uint32_t TDR;      // 0x28
} USART_TypeDef;
#endif

// 外设指针
#define RCC     ((RCC_TypeDef *)RCC_BASE)
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)GPIOD_BASE)
#if UART_ENABLE
#define USART1  ((USART_TypeDef *)USART1_BASE)
#define USART2  ((USART_TypeDef *)USART2_BASE)
#endif

// 获取GPIO端口指针
GPIO_TypeDef* get_gpio_port(char port) {
    switch(port) {
        case 'A': return GPIOA;
        case 'B': return GPIOB;
        case 'C': return GPIOC;
        case 'D': return GPIOD;
        default: return GPIOC;
    }
}

// 简单延时函数
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        for(uint32_t j = 0; j < DELAY_FACTOR; j++) {
            __asm__("nop");
        }
    }
}

// 电源控制初始化（最关键！）
void init_power_control(void) {
    // 1. PC6 - 主电源控制 (CON_POW_CPU)
    uint32_t enable_bit = RCC_GPIO_ENABLE_BIT(POWER_CPU_PORT);
    RCC->AHBENR |= (1UL << enable_bit);
    
    GPIO_TypeDef* gpio = get_gpio_port(POWER_CPU_PORT);
    uint32_t pin = POWER_CPU_PIN;
    gpio->MODER &= ~(3UL << (pin * 2));
    gpio->MODER |= (1UL << (pin * 2));
    if(POWER_CPU_LEVEL) {
        gpio->BSRR = (1UL << pin);  // 拉高
    } else {
        gpio->BRR = (1UL << pin);
    }
    
    // 2. PA3 - LCD电源控制 (CON_POW_LCD)
    enable_bit = RCC_GPIO_ENABLE_BIT(POWER_LCD_PORT);
    RCC->AHBENR |= (1UL << enable_bit);
    
    gpio = get_gpio_port(POWER_LCD_PORT);
    pin = POWER_LCD_PIN;
    gpio->MODER &= ~(3UL << (pin * 2));
    gpio->MODER |= (1UL << (pin * 2));
    if(POWER_LCD_LEVEL) {
        gpio->BSRR = (1UL << pin);  // 拉高
    } else {
        gpio->BRR = (1UL << pin);
    }
    
    // 3. PC5 - RF电源控制 (CON_POW_RF)  
    enable_bit = RCC_GPIO_ENABLE_BIT(POWER_RF_PORT);
    RCC->AHBENR |= (1UL << enable_bit);
    
    gpio = get_gpio_port(POWER_RF_PORT);
    pin = POWER_RF_PIN;
    gpio->MODER &= ~(3UL << (pin * 2));
    gpio->MODER |= (1UL << (pin * 2));
    if(POWER_RF_LEVEL) {
        gpio->BSRR = (1UL << pin);  // 拉高
    } else {
        gpio->BRR = (1UL << pin);
    }
}

// LED初始化
void init_leds(void) {
    // 开启LED1端口时钟
    uint32_t enable_bit = RCC_GPIO_ENABLE_BIT(LED1_PORT);
    RCC->AHBENR |= (1UL << enable_bit);
    
    // 开启LED2端口时钟（如果不同端口）
    enable_bit = RCC_GPIO_ENABLE_BIT(LED2_PORT);
    RCC->AHBENR |= (1UL << enable_bit);
    
    // 配置LED1为输出
    GPIO_TypeDef* gpio1 = get_gpio_port(LED1_PORT);
    uint32_t pin1 = LED1_PIN;
    gpio1->MODER &= ~(3UL << (pin1 * 2));
    gpio1->MODER |= (1UL << (pin1 * 2));
    
    // 配置LED2为输出
    GPIO_TypeDef* gpio2 = get_gpio_port(LED2_PORT);
    uint32_t pin2 = LED2_PIN;
    gpio2->MODER &= ~(3UL << (pin2 * 2));
    gpio2->MODER |= (1UL << (pin2 * 2));
    
    // 初始状态：关闭LED
    if(LED_ACTIVE_LOW) {
        gpio1->BSRR = (1UL << pin1);  // 高电平=灭
        gpio2->BSRR = (1UL << pin2);
    } else {
        gpio1->BRR = (1UL << pin1);   // 低电平=灭
        gpio2->BRR = (1UL << pin2);
    }
}

// 切换LED状态
void toggle_leds(void) {
    GPIO_TypeDef* gpio1 = get_gpio_port(LED1_PORT);
    GPIO_TypeDef* gpio2 = get_gpio_port(LED2_PORT);
    uint32_t pin1 = LED1_PIN;
    uint32_t pin2 = LED2_PIN;
    
    // 读取当前状态并翻转
    if(gpio1->ODR & (1UL << pin1)) {
        gpio1->BRR = (1UL << pin1);   // 当前高，设为低
    } else {
        gpio1->BSRR = (1UL << pin1);  // 当前低，设为高
    }
    
    if(gpio2->ODR & (1UL << pin2)) {
        gpio2->BRR = (1UL << pin2);
    } else {
        gpio2->BSRR = (1UL << pin2);
    }
}

#if UART_ENABLE

#if UART_MODE == 1
// 软件模拟串口 - 简单可靠
void init_uart(void) {
    // 开启GPIO时钟
    uint32_t enable_bit = RCC_GPIO_ENABLE_BIT(SOFT_TX_PORT);
    RCC->AHBENR |= (1UL << enable_bit);
    
    // 配置为输出
    GPIO_TypeDef* gpio = get_gpio_port(SOFT_TX_PORT);
    uint32_t pin = SOFT_TX_PIN;
    gpio->MODER &= ~(3UL << (pin * 2));
    gpio->MODER |= (1UL << (pin * 2));
    
    // 初始状态为高（串口空闲状态）
    gpio->BSRR = (1UL << pin);
}

// 软件串口发送单个字符
void uart_send_char(char c) {
    GPIO_TypeDef* gpio = get_gpio_port(SOFT_TX_PORT);
    uint32_t pin = SOFT_TX_PIN;
    uint32_t bit_delay = SYSTEM_CLOCK_HZ / UART_BAUDRATE / 16;
    
    // 起始位（低电平）
    gpio->BRR = (1UL << pin);
    for(uint32_t i = 0; i < bit_delay; i++) __asm__("nop");
    
    // 发送8个数据位
    for(int bit = 0; bit < 8; bit++) {
        if(c & (1 << bit)) {
            gpio->BSRR = (1UL << pin);  // 发送1
        } else {
            gpio->BRR = (1UL << pin);    // 发送0
        }
        for(uint32_t i = 0; i < bit_delay; i++) __asm__("nop");
    }
    
    // 停止位（高电平）
    gpio->BSRR = (1UL << pin);
    for(uint32_t i = 0; i < bit_delay; i++) __asm__("nop");
}

#else
// 硬件UART2 (PD5)
void init_uart(void) {
    // 开启GPIOD和USART2时钟
    RCC->AHBENR |= (1UL << 20);   // GPIOD
    RCC->APB1ENR |= (1UL << 17);  // USART2
    
    // PD5配置为复用功能（TX）
    GPIO_TypeDef* gpio = get_gpio_port(UART2_TX_PORT);
    uint32_t pin = UART2_TX_PIN;
    
    // 设为复用模式
    gpio->MODER &= ~(3UL << (pin * 2));
    gpio->MODER |= (2UL << (pin * 2));
    
    // 设置复用功能为AF0（USART2）
    if(pin < 8) {
        gpio->AFR[0] &= ~(0xFUL << (pin * 4));
        gpio->AFR[0] |= (0UL << (pin * 4));  // AF0
    } else {
        gpio->AFR[1] &= ~(0xFUL << ((pin - 8) * 4));
        gpio->AFR[1] |= (0UL << ((pin - 8) * 4));
    }
    
    // 配置USART2
    USART2->BRR = 278;  // 32MHz / 115200
    USART2->CR1 = (1UL << 3) | (1UL << 0);  // TE | UE
}

// 发送单个字符
void uart_send_char(char c) {
    while(!(USART2->ISR & (1UL << 7)));  // TXE
    USART2->TDR = c;
}
#endif

// 发送字符串
void send_string(const char* str) {
    while(*str) {
        uart_send_char(*str++);
    }
}

// 发送16进制数
void send_hex(uint8_t val) {
    static const char* hex = "0123456789ABCDEF";
    uart_send_char(hex[val >> 4]);
    uart_send_char(hex[val & 0x0F]);
}
#endif

// 主函数
int main(void) {
    // ========== 步骤1: 紧急电源控制（最关键！必须150ms内） ==========
    init_power_control();
    
    // ========== 步骤2: 初始化LED ==========
    init_leds();
    
    // ========== 步骤3: 初始化串口（如果启用） ==========
    #if UART_ENABLE
    init_uart();
    send_string("\r\n===== WB32L003 QFN32 Test =====\r\n");
    send_string("Power: PC6=CPU PC5=RF PA3=LCD\r\n");
    send_string("LEDs: PC0=RED PC1=GREEN\r\n");
    #if UART_MODE == 1
    send_string("UART: Software on P");
    uart_send_char(SOFT_TX_PORT);
    uart_send_char('0' + SOFT_TX_PIN);
    #else
    send_string("UART: Hardware PD5/UART2_TX");
    #endif
    send_string("\r\n");
    send_string("Starting LED flash test...\r\n");
    #endif
    
    // ========== 步骤4: 快闪3次表示启动成功 ==========
    for(int i = 0; i < 3; i++) {
        toggle_leds();
        delay_ms(200);
        toggle_leds();
        delay_ms(200);
    }
    
    #if UART_ENABLE
    send_string("Initialization complete!\r\n");
    #endif
    
    // ========== 步骤5: 主循环 - LED闪烁 ==========
    uint32_t counter = 0;
    while(1) {
        // 切换LED状态
        toggle_leds();
        delay_ms(500);
        
        // 串口心跳输出
        #if UART_ENABLE
        if(++counter % 4 == 0) {  // 每2秒输出一次
            send_string("[HB] Counter: 0x");
            send_hex((counter >> 8) & 0xFF);
            send_hex(counter & 0xFF);
            send_string("\r\n");
        }
        #endif
    }
    
    return 0;
}

// 错误处理（如果需要）
void Error_Handler(void) {
    while(1) {
        // 快速闪烁表示错误
        toggle_leds();
        delay_ms(100);
    }
}