# WB32L003 耳放固件开机问题分析文档

## 1. 当前问题

设备在烧录固件后无法开机，具体表现：
- USB 供电后屏幕不亮
- 按下板上唯一的按钮（PB0）无反应
- 已尝试 v3.3.0 版本（基于 o3 晶体管分析的正确逻辑）仍然无法点亮

## 2. 硬件控制逻辑实现历史

### v3.0.x - 基于 o3 初始分析
- PC6 = LOW 开启电源（正确！）
- PA3 = HIGH 开启 LCD
- 结果：不工作

### v3.2.x - 基于洪哥 RTF 文档
- PC6 = HIGH 开启电源
- PA3 = HIGH 开启 LCD  
- 结果：不工作

### v3.3.0 - 基于 o3 晶体管分析
- PC6 = LOW 开启电源（P-MOS 晶体管）
- PA3 = LOW 开启 LCD（PNP 晶体管）
- PB1 = LOW 开启背光（PNP 晶体管）
- 结果：仍然不工作

## 3. o3 的晶体管分析总结

```
PC6 (CON_POW_CPU) 控制逻辑：
- PC6 → Q6 (NPN) → Q5 (P-MOS) → 电源
- PC6 LOW → Q6 OFF → Q5 栅极被拉低 → Q5 导通 → 电源开启
- PC6 HIGH → Q6 ON → Q5 栅极高电平 → Q5 关闭 → 电源关闭

PA3 (CON_POW_LCD) 控制逻辑：
- PA3 → 限流电阻 → Q7 (PNP) → LCD 电源
- PA3 LOW → Q7 导通 → LCD 供电
- PA3 HIGH → Q7 关闭 → LCD 断电

PB1 (CLD_BL1) 控制逻辑：
- PB1 → 限流电阻 → Q7 (PNP) → 背光电源
- PB1 LOW → Q7 导通 → 背光供电
- PB1 HIGH → Q7 关闭 → 背光断电
```

## 4. 极简测试固件

```c
/* minimal_pc6.c - 最简单的测试，只保持电源开启 */
int main(void) {
    // 直接寄存器操作，无 HAL 依赖
    // 启用 GPIOC 时钟
    RCC->AHBENR |= (1UL << 19);  // GPIOCEN
    
    // 设置 PC6 为输出
    GPIOC->MODER &= ~(3UL << 12);
    GPIOC->MODER |= (1UL << 12);
    
    // 立即设置 PC6 为 LOW
    GPIOC->BRR = (1UL << 6);
    
    // 同时设置 PA3 和 PB1 为 LOW
    RCC->AHBENR |= (1UL << 17);  // GPIOAEN
    RCC->AHBENR |= (1UL << 18);  // GPIOBEN
    
    // PA3 输出 LOW
    GPIOA->MODER &= ~(3UL << 6);
    GPIOA->MODER |= (1UL << 6);
    GPIOA->BRR = (1UL << 3);
    
    // PB1 输出 LOW
    GPIOB->MODER &= ~(3UL << 2);
    GPIOB->MODER |= (1UL << 2);
    GPIOB->BRR = (1UL << 1);
    
    // 主循环 - 保持 PC6 LOW 并闪烁 LED
    while(1) {
        GPIOC->BRR = (1UL << 6);  // 保持 PC6 LOW
        // 闪烁红色 LED (PA7)
    }
}
```

## 5. 已测试的所有电源控制组合

共 16 种组合，其中理论上应该工作的：
- 组合 1: PC6=LOW, PA3=LOW, PB1=LOW, PB2=LOW （o3 分析应该正确）
- 组合 2: PC6=LOW, PA3=LOW, PB1=LOW, PB2=HIGH（o3 分析应该正确）

## 6. 可能的问题分析

### 6.1 时序问题
- MCU 启动后可能已经太晚设置 PC6
- 电源管理芯片可能需要特定的上电时序

### 6.2 硬件初始化
- 可能存在未记录的电源控制引脚（如 PD0, PD1）
- 晶振可能未启动导致 MCU 无法正常运行

### 6.3 固件加载问题
- Flash 基址已修正为 0x00000000（WB32L003 特定）
- 向量表可能需要特殊配置

### 6.4 电源按钮逻辑
- PB0 可能不只是简单的输入按钮
- 可能需要特定的按钮按压时序配合 PC6

### 6.5 启动模式
- 设备可能处于 ISP 模式
- 可能需要特定的 BOOT 引脚配置

## 7. 调试建议

1. **屏幕背光测试**：如果屏幕背光能亮起，说明电源控制正常
2. **按钮测试**：尝试在不同时机按住 PB0
3. **电压测量**：测量关键点的电压（如果有万用表）
4. **ISP 模式**：检查是否需要特殊的启动配置

## 8. 关键问题

1. **MCU 是否在运行？** - 通过屏幕背光或其他可见反馈确认
2. **电源是否真的开启？** - PC6 LOW 后是否维持供电
3. **是否有其他电源控制？** - PD0/PD1 或其他未记录的引脚
4. **启动时序是否关键？** - 是否需要在特定时间窗口内设置 PC6

## 9. 代码片段对比

### main.c 中的关键初始化
```c
/* 在 main() 最开始立即设置 PC6 LOW */
__HAL_RCC_GPIOC_CLK_ENABLE();
GPIO_InitTypeDef GPIO_InitStruct = {0};
GPIO_InitStruct.Pin = GPIO_PIN_6;  // PC6
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);  // LOW = 保持电源开启！
```

### hardware_control.c 中的电源控制
```c
void HW_SetMainPower(bool enable)
{
    /* PC6 控制 Q5 P-MOS 通过 Q6 NPN 晶体管
     * PC6 LOW -> Q6 OFF -> Q5 栅极拉低 -> Q5 ON -> 电源开启
     * PC6 HIGH -> Q6 ON -> Q5 栅极高 -> Q5 OFF -> 电源关闭
     */
    HAL_GPIO_WritePin(CON_POW_CPU_PORT, CON_POW_CPU_PIN, 
                      enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
```

## 10. 请 o3 帮助分析

1. 是否有遗漏的硬件初始化步骤？
2. PC6 设置 LOW 的时机是否足够早？
3. 是否需要其他引脚配合才能维持电源？
4. PB0 按钮是否参与电源控制逻辑？
5. 有没有可能是硬件损坏或者需要特殊的启动条件？