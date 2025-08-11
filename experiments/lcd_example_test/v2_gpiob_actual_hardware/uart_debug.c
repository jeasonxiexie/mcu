/**
 * @file uart_debug.c
 * @brief Simple UART debug output for v2
 */

#include "hal/wb32l003_hal.h"

// UART handle
static UART_HandleTypeDef huart1;

// Initialize UART for debug output
// Using UART1: PA9(TX), PA10(RX)
void UART_Debug_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Enable UART1 and GPIOA clocks
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    // Configure PA9 (TX) and PA10 (RX)
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Configure UART: 115200, 8N1
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        // Initialization error - flash red LED rapidly
        while(1)
        {
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
            HAL_Delay(50);
        }
    }
}

// Send string via UART
void UART_Debug_Print(const char* str)
{
    // Calculate string length manually
    uint16_t len = 0;
    while (str[len] != '\0' && len < 1000) len++;
    
    HAL_UART_Transmit(&huart1, (uint8_t*)str, len, 1000);
}

// Send string with newline
void UART_Debug_PrintLn(const char* str)
{
    UART_Debug_Print(str);
    UART_Debug_Print("\r\n");
}

// Print a hex value
void UART_Debug_PrintHex(const char* label, uint32_t value)
{
    UART_Debug_Print(label);
    UART_Debug_Print(": 0x");
    
    // Convert to hex manually
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[9]; // 8 hex digits + null
    for (int i = 7; i >= 0; i--)
    {
        buffer[i] = hex_chars[value & 0xF];
        value >>= 4;
    }
    buffer[8] = '\0';
    
    UART_Debug_Print(buffer);
    UART_Debug_Print("\r\n");
}