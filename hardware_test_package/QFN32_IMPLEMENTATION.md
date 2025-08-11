# WB32L003 QFN32 硬件测试实现方案

## 1. 问题背景

### 1.1 初始问题
- 硬件人员反馈：simple_test.hex文件无法让LED闪烁
- 测量结果：
  - PC6 = 0V（电源控制失效）
  - PC0/PC1 = 1.5V高阻（LED控制失效）
  - 最初误解：以为QFN32没有PC引脚

### 1.2 关键发现
- RTF文档是基于48引脚封装编写的
- 实际硬件使用QFN32封装（32引脚）
- 通过查阅数据手册确认：QFN32确实有PC0-PC7引脚

## 2. QFN32引脚映射（GPT5验证版）

### 2.1 关键引脚分配
```
LED控制：
- PC0 → LED_RED（低电平点亮）→ Pin 17
- PC1 → LED_GREEN（低电平点亮）→ Pin 18

调试串口：
- PD5 → UART2_TX → Pin 30（推荐）
- PB4 → 软件串口备选 → Pin 12

电源控制（三路必须全部使能）：
- PC6 → CON_POW_CPU（开机=HIGH）→ Pin 23
- PA3 → CON_POW_LCD（开机=HIGH）→ Pin 7
- PC5 → CON_POW_RF（开机=HIGH）→ Pin 22
```

### 2.2 为什么原版固件不工作
1. **串口引脚错误**：PA9在QFN32上不存在
2. **电源控制不完整**：只控制了PC6，未控制PA3和PC5
3. **缺少GPIOD支持**：代码中未定义GPIOD寄存器

## 3. 修正后的实现代码

### 3.1 配置文件 (config.h)
```c
#ifndef __CONFIG_H
#define __CONFIG_H

// ============================================================
//                     电源控制配置（最重要！）
// ============================================================
// QFN32版本需要控制三路电源
// PC6 = CON_POW_CPU (Pin 23) - 主电源控制
#define POWER_CPU_PORT      'C'     // 端口
#define POWER_CPU_PIN       6       // Pin 23
#define POWER_CPU_LEVEL     1       // 1=高电平使能

// PA3 = CON_POW_LCD (Pin 7) - LCD电源控制
#define POWER_LCD_PORT      'A'     // 端口
#define POWER_LCD_PIN       3       // Pin 7
#define POWER_LCD_LEVEL     1       // 1=高电平使能

// PC5 = CON_POW_RF (Pin 22) - RF电源控制  
#define POWER_RF_PORT       'C'     // 端口
#define POWER_RF_PIN        5       // Pin 22
#define POWER_RF_LEVEL      1       // 1=高电平使能

// ============================================================
//                        LED配置
// ============================================================
// PC0 = LED_RED (Pin 17) - 红色LED
#define LED1_PORT           'C'     // 端口
#define LED1_PIN            0       // Pin 17
  
// PC1 = LED_GREEN (Pin 18) - 绿色LED
#define LED2_PORT           'C'     // 端口
#define LED2_PIN            1       // Pin 18

// LED极性设置
#define LED_ACTIVE_LOW      1       // 1=低电平点亮(确认), 0=高电平点亮

// ============================================================
//                       串口调试配置
// ============================================================
#define UART_ENABLE         1       // 1=启用串口输出, 0=禁用
#define UART_MODE           1       // 0=硬件UART2, 1=软件模拟

// 硬件UART2配置 (PD5 = Pin 30)
#define UART2_TX_PORT       'D'     // 串口TX端口
#define UART2_TX_PIN        5       // Pin 30
#define UART_BAUDRATE       115200  // 波特率

// 软件模拟串口配置 (使用任意GPIO)
#define SOFT_TX_PORT        'B'     // 可选PB4或其他空闲引脚
#define SOFT_TX_PIN         4       // Pin 12

// ============================================================
//                       系统配置
// ============================================================
#define SYSTEM_CLOCK_HZ     32000000UL  // 32MHz系统时钟
#define DELAY_FACTOR        4000        // 延时系数（根据时钟调整）

// ============================================================
//                    寄存器基地址（不要修改）
// ============================================================
#define RCC_BASE            0x40021000UL
#define GPIOA_BASE          0x48000000UL
#define GPIOB_BASE          0x48000400UL
#define GPIOC_BASE          0x48000800UL
#define GPIOD_BASE          0x48000C00UL  // 新增GPIOD
#define USART1_BASE         0x40013800UL
#define USART2_BASE         0x40004400UL  // 新增USART2

// ============================================================
//                    辅助宏定义（不要修改）
// ============================================================
// GPIO端口选择
#define GPIO_BASE(port) \
    ((port) == 'A' ? GPIOA_BASE : \
     (port) == 'B' ? GPIOB_BASE : \
     (port) == 'C' ? GPIOC_BASE : \
     (port) == 'D' ? GPIOD_BASE : 0)

// RCC时钟使能位
#define RCC_GPIO_ENABLE_BIT(port) \
    ((port) == 'A' ? 17 : \
     (port) == 'B' ? 18 : \
     (port) == 'C' ? 19 : \
     (port) == 'D' ? 20 : 0)

#endif /* __CONFIG_H */
```

### 3.2 主程序关键修改 (main.c)

#### 3.2.1 添加GPIOD支持
```c
// 外设指针
#define RCC     ((RCC_TypeDef *)RCC_BASE)
#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)GPIOD_BASE)  // 新增GPIOD

// 获取GPIO端口指针
GPIO_TypeDef* get_gpio_port(char port) {
    switch(port) {
        case 'A': return GPIOA;
        case 'B': return GPIOB;
        case 'C': return GPIOC;
        case 'D': return GPIOD;  // 新增GPIOD
        default: return GPIOC;
    }
}
```

#### 3.2.2 三路电源控制初始化
```c
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
```

#### 3.2.3 软件模拟串口实现（更可靠）
```c
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
#endif
```

#### 3.2.4 主函数启动序列
```c
int main(void) {
    // ========== 步骤1: 紧急电源控制（最关键！必须150ms内） ==========
    init_power_control();  // 同时控制三路电源
    
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
    
    // ========== 步骤5: 主循环 - LED闪烁 ==========
    while(1) {
        toggle_leds();
        delay_ms(500);
    }
}
```

## 4. 关键修正点总结

### 4.1 硬件层面
1. **封装差异识别**：从48引脚到QFN32的正确映射
2. **电源管理完整性**：三路电源都必须控制（PC6/PA3/PC5）
3. **串口引脚可用性**：PA9不存在，改用PD5或软件模拟

### 4.2 软件层面
1. **GPIOD寄存器支持**：添加0x48000C00UL基地址
2. **软件串口实现**：避免硬件UART配置复杂性
3. **启动时序保证**：150ms内完成所有电源控制

## 5. 测试验证步骤

### 5.1 硬件测试要点
```
1. 上电后立即测量：
   - PC6应为3.3V（主电源）
   - PC5应为3.3V（RF电源）
   - PA3应为3.3V（LCD电源）

2. LED状态验证：
   - PC0和PC1应在0V和3.3V之间切换
   - 低电平时LED点亮

3. 串口输出检查：
   - 软件串口：PB4输出
   - 硬件串口：PD5输出
   - 波特率115200，8N1
```

### 5.2 预期结果
1. 上电后LED快闪3次（启动成功标志）
2. 之后LED以500ms间隔交替闪烁
3. 串口输出启动信息和心跳包

## 6. 故障排除

### 6.1 如果LED仍不亮
- 检查PC6/PC5/PA3是否都为高电平
- 验证LED连接极性（低电平点亮）
- 确认PC0/PC1引脚焊接正常

### 6.2 如果串口无输出
- 优先使用软件模拟串口（更简单可靠）
- 检查USB-UART转换器RX连接到MCU的TX
- 确认波特率设置为115200

### 6.3 如果系统不稳定
- 确保三路电源都已使能
- 检查电源纹波和去耦电容
- 验证晶振频率是否为32MHz

## 7. 编译命令

```bash
# 编译
arm-none-eabi-gcc -c source/main.c -o build/main.o \
    -mcpu=cortex-m0 -mthumb -Os \
    -ffunction-sections -fdata-sections -Wall

arm-none-eabi-gcc -c source/startup_wb32l003.s -o build/startup.o \
    -mcpu=cortex-m0 -mthumb

# 链接
arm-none-eabi-gcc build/startup.o build/main.o \
    -o build/test.elf \
    -T source/wb32l003.ld \
    -mcpu=cortex-m0 -mthumb \
    -Wl,--gc-sections \
    -nostdlib -nodefaultlibs -nostartfiles

# 生成HEX文件
arm-none-eabi-objcopy -O ihex build/test.elf build/test.hex
```

## 8. 结论

本方案基于QFN32封装的实际硬件配置，修正了原RTF文档基于48引脚的错误假设。通过完整的三路电源控制、正确的GPIO映射和可靠的软件串口实现，应该能够成功驱动LED并输出调试信息。

关键成功因素：
1. 识别并适配正确的封装类型
2. 完整控制所有必需的电源轨
3. 使用存在且可用的引脚
4. 简化串口实现降低复杂度