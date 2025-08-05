/*
 * 基于项目描述的测试 - CON_POW/CON_LCD 高电平开机
 */

#include "main.h"

int main(void) {
    // 早期电源锁存仍然需要（保持 150ms 窗口）
    // PC6 已在 Early_PowerLatch() 中处理
    
    HAL_Init();
    
    // 启用所有 GPIO 时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    RCC->AHBENR |= (1UL << 20);  // GPIOD
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    // 根据项目描述设置引脚
    // CON_POW - 假设是 PC6
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    // CON_LCD - 假设是 PA3
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // MUTE - 需要找到对应引脚
    // Green/Red - 外部 LED 控制
    
    // 背光控制
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // 根据项目描述：高电平开机！
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   // CON_POW = HIGH
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);   // CON_LCD = HIGH
    
    // 背光测试 - 尝试不同组合
    uint8_t state = 0;
    
    while(1) {
        // 保持电源高电平
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
        
        // 每秒切换背光状态
        HAL_Delay(1000);
        
        switch(state) {
            case 0:  // 尝试 PB1=LOW, PB2=HIGH
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
                break;
                
            case 1:  // 尝试 PB1=HIGH, PB2=HIGH
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
                break;
                
            case 2:  // 尝试 PB1=LOW, PB2=LOW
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
                break;
                
            case 3:  // 尝试 PB1=HIGH, PB2=LOW
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
                break;
        }
        
        state = (state + 1) & 3;
    }
}