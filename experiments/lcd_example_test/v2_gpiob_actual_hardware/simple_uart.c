/**
 * @file simple_uart.c
 * @brief 简单的UART调试输出，直接操作寄存器
 */

#include "wb32l003.h"

// USART1 寄存器直接定义
#define USART1_BASE     0x40013800UL
#define USART1_SR       (*(volatile uint32_t*)(USART1_BASE + 0x00))
#define USART1_DR       (*(volatile uint32_t*)(USART1_BASE + 0x04))
#define USART1_BRR      (*(volatile uint32_t*)(USART1_BASE + 0x08))
#define USART1_CR1      (*(volatile uint32_t*)(USART1_BASE + 0x0C))

// USART 寄存器位定义
#define USART_SR_TXE    (1UL << 7)  // 发送数据寄存器空
#define USART_CR1_TE    (1UL << 3)  // 发送使能
#define USART_CR1_UE    (1UL << 13) // USART使能

// RCC 寄存器位定义
#define RCC_APB2ENR_USART1EN  (1UL << 14)

// UART1 使用 PA9(TX), PA10(RX)
void Simple_UART_Init(void)
{
    // 1. 使能时钟
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;    // GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // UART1时钟
    
    // 2. 配置PA9为复用功能（UART1_TX）
    GPIOA->MODER &= ~(3UL << (9 * 2));    // 清除模式位
    GPIOA->MODER |= (2UL << (9 * 2));     // 复用功能模式
    GPIOA->AFR[1] &= ~(0xF << ((9-8) * 4));
    GPIOA->AFR[1] |= (1 << ((9-8) * 4));  // AF1 for UART1
    
    // 3. 配置UART1
    // 当前 SystemCoreClock ~ 32MHz（参见 system_wb32l003.c）
    // 波特率 = Fck / (16 * USARTDIV)
    // 115200 = 32000000 / (16 * USARTDIV) => USARTDIV ≈ 17.36
    // 取 17 更接近目标波特率（~117647）
    USART1_BRR = 17;  // 约 115200 bps
    
    // 4. 使能发送器
    USART1_CR1 = USART_CR1_TE | USART_CR1_UE; // 使能发送和UART
}

// 发送一个字节
void Simple_UART_SendByte(uint8_t byte)
{
    // 等待发送缓冲区空
    while (!(USART1_SR & USART_SR_TXE));
    
    // 发送数据
    USART1_DR = byte;
}

// 发送字符串
void Simple_UART_Print(const char* str)
{
    while (*str)
    {
        Simple_UART_SendByte(*str++);
    }
}

// 发送字符串并换行
void Simple_UART_PrintLn(const char* str)
{
    Simple_UART_Print(str);
    Simple_UART_Print("\r\n");
}

// 发送十六进制数
void Simple_UART_PrintHex8(uint8_t value)
{
    const char hex[] = "0123456789ABCDEF";
    Simple_UART_SendByte(hex[(value >> 4) & 0xF]);
    Simple_UART_SendByte(hex[value & 0xF]);
}

void Simple_UART_PrintHex32(const char* label, uint32_t value)
{
    Simple_UART_Print(label);
    Simple_UART_Print(": 0x");
    Simple_UART_PrintHex8((value >> 24) & 0xFF);
    Simple_UART_PrintHex8((value >> 16) & 0xFF);
    Simple_UART_PrintHex8((value >> 8) & 0xFF);
    Simple_UART_PrintHex8(value & 0xFF);
    Simple_UART_Print("\r\n");
}