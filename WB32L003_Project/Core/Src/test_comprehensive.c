/*
 * 综合测试 - 结合 o3 分析和项目描述
 */

#include "main.h"
#include "pin_config.h"

// 简单延时
static void simple_delay(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        for(volatile uint32_t j = 0; j < 8000; j++) {
            __NOP();
        }
    }
}

int main(void) {
    /* PC6 已在 Early_PowerLatch() 中设置为 LOW - 这是维持电源的关键！ */
    
    HAL_Init();
    
    /* 启用所有 GPIO 时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    RCC->AHBENR |= (1UL << 20);  // GPIOD
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    /* 1. 电源控制 - 保持 o3 的逻辑 */
    GPIO_InitStruct.Pin = CON_POW_CPU_PIN;  // PC6
    HAL_GPIO_Init(CON_POW_CPU_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_POW_CPU_PORT, CON_POW_CPU_PIN, GPIO_PIN_RESET);  // LOW = ON
    
    /* 2. LCD 电源 */
    GPIO_InitStruct.Pin = CON_POW_LCD_PIN;  // PA3
    HAL_GPIO_Init(CON_POW_LCD_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, GPIO_PIN_RESET);  // 先试 LOW
    
    /* 3. MUTE 控制 */
    GPIO_InitStruct.Pin = CON_MUTE_PIN;  // PA4
    HAL_GPIO_Init(CON_MUTE_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_MUTE_PORT, CON_MUTE_PIN, GPIO_PIN_SET);  // HIGH = unmute
    
    /* 4. LED 控制 - PC0/PC1 共阳极 */
    GPIO_InitStruct.Pin = LED_RED_PIN | LED_GREEN_PIN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /* 5. 背光控制 */
    GPIO_InitStruct.Pin = CLD_BL1_PIN | CLD_BL2_PIN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* 6. 5V 电源（音频放大器） */
    GPIO_InitStruct.Pin = CON_POW_RF_PIN;  // PC5
    HAL_GPIO_Init(CON_POW_RF_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(CON_POW_RF_PORT, CON_POW_RF_PIN, GPIO_PIN_SET);  // 开启 5V
    
    /* 初始状态：绿灯亮 */
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);  // LOW = ON
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);        // HIGH = OFF
    
    /* 背光全亮 */
    HAL_GPIO_WritePin(GPIOB, CLD_BL1_PIN, GPIO_PIN_RESET);  // LOW = ON
    HAL_GPIO_WritePin(GPIOB, CLD_BL2_PIN, GPIO_PIN_SET);    // HIGH = bright
    
    uint8_t test_state = 0;
    
    while(1) {
        /* 保持主电源 */
        HAL_GPIO_WritePin(CON_POW_CPU_PORT, CON_POW_CPU_PIN, GPIO_PIN_RESET);
        
        simple_delay(2000);  // 2秒
        
        switch(test_state) {
            case 0:  // 绿灯亮，LCD 电源 LOW
                HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, GPIO_PIN_RESET);
                break;
                
            case 1:  // 红灯亮，LCD 电源 HIGH（测试项目描述）
                HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, GPIO_PIN_SET);
                break;
                
            case 2:  // 两灯都亮，切换背光
                HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
                HAL_GPIO_TogglePin(GPIOB, CLD_BL2_PIN);
                break;
                
            case 3:  // 红灯闪烁（模拟低电量）
                for(int i = 0; i < 4; i++) {
                    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
                    simple_delay(200);
                    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
                    simple_delay(50);
                }
                break;
        }
        
        test_state = (test_state + 1) & 3;
    }
}