/*
 * Power control test - cycles through combinations to find what works
 */

#include "main.h"
#include "pin_config.h"

// Simple delay
void test_delay(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        for(uint32_t j = 0; j < 1000; j++) {
            __NOP();
        }
    }
}

int main(void) {
    // Initialize HAL
    HAL_Init();
    
    // Enable all GPIO clocks FIRST
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    // Manual GPIOD enable
    RCC->AHBENR |= (1UL << 20);
    
    // Configure all pins as outputs
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    // PC6 - Main power
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    // PA3 - LCD power
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // PB1 - Backlight enable
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // PB2 - Backlight brightness
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // LEDs
    GPIO_InitStruct.Pin = GPIO_PIN_7;  // PA7 - Red
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_3;  // PD3 - Green
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // Additional test pins
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;  // PD0, PD1
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    uint8_t state = 0;
    
    // Main test loop
    while(1) {
        switch(state) {
            case 0:  // o3 logic - ALL LOW
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);  // PC6 LOW
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);  // PA3 LOW
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);  // PB1 LOW
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);    // PB2 HIGH
                // Red LED on
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
                break;
                
            case 1:  // 洪哥 logic - ALL HIGH
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);    // PC6 HIGH
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);    // PA3 HIGH
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);    // PB1 HIGH
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);    // PB2 HIGH
                // Green LED on
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
                break;
                
            case 2:  // Mixed - PC6 LOW only
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);  // PC6 LOW
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);    // PA3 HIGH
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);    // PB1 HIGH
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);  // PB2 LOW
                // Both LEDs on
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
                break;
                
            case 3:  // Test PD0/PD1
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
                test_delay(500);
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
                // Toggle LEDs
                HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
                HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_3);
                break;
                
            case 4:  // Pulse PC6
                for(int i = 0; i < 5; i++) {
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
                    test_delay(200);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
                    test_delay(200);
                }
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);  // End LOW
                break;
        }
        
        test_delay(2000);
        state++;
        if(state > 4) state = 0;
    }
}