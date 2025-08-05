/**
 * 紧急修复程序 - 根据原理图修正电源控制
 * 
 * 关键发现：
 * 1. P_CTL 控制主电源 MOSFET (Q3)
 * 2. 必须先设置 P_CTL 为高电平才能让系统正常工作
 */

#include "wb32l003.h"

// 根据原理图，P_CTL 很可能连接到某个未使用的 GPIO
// 需要从原理图确认具体是哪个引脚
// 暂时假设是 PA2（需要确认）
#define P_CTL_PIN   GPIO_PIN_2
#define P_CTL_PORT  GPIOA

int main(void)
{
    // 系统初始化
    HAL_Init();
    
    // 立即配置 P_CTL 为输出并拉高（开启主电源）
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = P_CTL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(P_CTL_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(P_CTL_PORT, P_CTL_PIN, GPIO_PIN_SET);  // 开启主电源！
    
    // 等待电源稳定
    HAL_Delay(100);
    
    // 配置 LED 用于调试（PD0）
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // 配置背光强制开启（PA1）
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
    
    // 配置 LCD 电源（如果有）
    GPIO_InitStruct.Pin = GPIO_PIN_8;  // PB8 - CON_LCD
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    
    // 简单的测试循环
    uint8_t counter = 0;
    while(1)
    {
        // LED 闪烁表示系统运行
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_0);
        
        // 每5次循环切换一次背光，测试是否有反应
        if(++counter >= 5)
        {
            counter = 0;
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
        }
        
        HAL_Delay(500);
    }
}