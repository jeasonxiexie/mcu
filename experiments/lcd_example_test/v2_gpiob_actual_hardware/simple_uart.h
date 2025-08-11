/**
 * @file simple_uart.h
 * @brief 简单UART接口
 */

#ifndef SIMPLE_UART_H
#define SIMPLE_UART_H

#include <stdint.h>

// 初始化UART（115200, 8N1）
void Simple_UART_Init(void);

// 发送一个字节
void Simple_UART_SendByte(uint8_t byte);

// 发送字符串
void Simple_UART_Print(const char* str);

// 发送字符串并换行
void Simple_UART_PrintLn(const char* str);

// 发送十六进制数
void Simple_UART_PrintHex8(uint8_t value);
void Simple_UART_PrintHex32(const char* label, uint32_t value);

#endif // SIMPLE_UART_H