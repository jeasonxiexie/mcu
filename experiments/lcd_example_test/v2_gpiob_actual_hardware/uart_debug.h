/**
 * @file uart_debug.h
 * @brief UART debug interface
 */

#ifndef UART_DEBUG_H
#define UART_DEBUG_H

#include <stdint.h>

// Initialize UART for debug output
void UART_Debug_Init(void);

// Send string via UART
void UART_Debug_Print(const char* str);

// Send string with newline
void UART_Debug_PrintLn(const char* str);

// Print a hex value
void UART_Debug_PrintHex(const char* label, uint32_t value);

#endif // UART_DEBUG_H