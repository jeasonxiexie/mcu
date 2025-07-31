# WB32L003 耳放固件调试分析文档

## 当前问题
固件 v3.2.1 无法开机，按下电源按钮后系统不响应

## 硬件配置对比

### 洪哥文档描述
```
电源管理：
- 主电源控制：PC6 (CON_POW_CPU)
  - 开机=HIGH
  - 关机=LOW
- LCD电源：PA3 (CON_POW_LCD)
  - 开机=HIGH
  - 关机=LOW
- 5V升压：PC5 (CON_POW_RF)
  - 开机=HIGH
  - 关机=LOW

用户界面：
- 电源开关：PB0 (SW_POW)
  - LOW=开机
  - HIGH=关机
- MODE按键：PD3 (KEY_STEREO)
  - 切换STEREO/MONO模式
```

### 当前固件实现 (v3.2.1)

#### 1. 引脚定义 (pin_config.h)
```c
/* Power management pins - Hardware team verified */
#define CON_POW_CPU_PIN         GPIO_PIN_6      // PC6 - Main power control (HIGH = ON, LOW = OFF)
#define CON_POW_CPU_PORT        GPIOC

#define CON_POW_RF_PIN          GPIO_PIN_5      // PC5 - 5V power control (for audio amp)
#define CON_POW_RF_PORT         GPIOC

#define CON_POW_LCD_PIN         GPIO_PIN_3      // PA3 - LCD power control
#define CON_POW_LCD_PORT        GPIOA

/* User Interface */
#define SW_POW_PIN              GPIO_PIN_0      // PB0 - SW_POW (Power button detection)
#define SW_POW_PORT             GPIOB

#define KEY_STEREO_PIN          GPIO_PIN_3      // PD3 - KEY_STEREO (Mode button)
#define KEY_STEREO_PORT         GPIOD

/* LED indicators */
#define LED_RED_PIN             GPIO_PIN_0      // PC0 - Red LED (Common anode, LOW = ON)
#define LED_RED_PORT            GPIOC
#define LED_GREEN_PIN           GPIO_PIN_1      // PC1 - Green LED (Common anode, LOW = ON)
#define LED_GREEN_PORT          GPIOC

/* Audio control */
#define CON_MUTE_PIN            GPIO_PIN_4      // PA4 - CON_MUTE (Low = mute, High = unmute)
#define CON_MUTE_PORT           GPIOA

#define CON_STEREO_PIN          GPIO_PIN_2      // PD2 - CON_STEREO (Low = stereo, High = mono)
#define CON_STEREO_PORT         GPIOD
```

#### 2. 电源控制函数 (hardware_control.c)
```c
void HW_SetMainPower(bool enable)
{
    /* Hardware team verified: PC6 controls main power
     * PC6 HIGH = Power ON
     * PC6 LOW = Power OFF
     */
    HAL_GPIO_WritePin(CON_POW_CPU_PORT, CON_POW_CPU_PIN, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void HW_SetLCDPower(bool enable)
{
    /* Hardware team verified: PA3 controls LCD power (HIGH = ON) */
    HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void HW_Set5VPower(bool enable)
{
    /* Hardware team verified: PC5 controls 5V boost (HIGH = ON) */
    HAL_GPIO_WritePin(CON_POW_RF_PORT, CON_POW_RF_PIN, enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
```

#### 3. 开机流程 (main.c)
```c
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    
    /* CRITICAL: Set PC6 HIGH immediately to maintain power! */
    /* This must be done before user releases power button */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_6;  // PC6
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);  // HIGH = Keep power ON
    
    GPIO_Init();
    ADC_Init();
    TIM_Init();
    
    HW_Init();
    UI_Init();
    Power_Init();
    
    // Use proper power-on sequence
    HW_PowerOnSequence();
    ...
}
```

#### 4. 电源开机序列 (hardware_control.c)
```c
void HW_PowerOnSequence(void)
{
    /* Hardware team power-on sequence:
     * 1. PC6 HIGH to maintain power after SW_POW release
     * 2. Mute audio first (PA4 LOW)
     * 3. Enable LCD power (PA3 HIGH)
     * 4. Initialize LCD and backlight
     * 5. Enable 5V boost (PC5 HIGH)
     * 6. Unmute audio (PA4 HIGH)
     */
    
    // 1. Ensure main power stays ON (PC6 must be HIGH)
    HW_SetMainPower(true);  // This sets PC6 HIGH
    HAL_Delay(10);
    
    // 2. Mute audio during power on
    HW_SetMute(true);  // PA4 LOW
    
    // 3. Enable LCD power
    HW_SetLCDPower(true);  // PA3 HIGH
    HAL_Delay(10);
    
    // 4. Initialize LCD
    ST7735_Init();
    // PB1 LOW to enable backlight power via Q7
    HAL_GPIO_WritePin(CLD_BL1_PORT, CLD_BL1_PIN, GPIO_PIN_RESET);
    // PB2 HIGH for bright backlight
    HAL_GPIO_WritePin(CLD_BL2_PORT, CLD_BL2_PIN, GPIO_PIN_SET);
    UI_ShowBootLogo();
    
    // 5. Enable 5V for audio amplifier
    HW_Set5VPower(true);  // PC5 HIGH
    HAL_Delay(50);  // Allow 5V to stabilize
    
    // 6. Unmute audio after power stable
    HW_SetMute(false);  // PA4 HIGH
    ...
}
```

## 可能的问题分析

### 1. PC6 电源控制逻辑
当前实现：
- PC6 = HIGH 保持开机
- PC6 = LOW 关机

但实际硬件可能是相反的逻辑。

### 2. 开机检测逻辑
代码假设：
- 用户按下 PB0（电源按钮）
- MCU 启动并立即设置 PC6 为 HIGH
- 用户释放按钮后，系统靠 PC6 维持供电

### 3. 实际硬件照片分析
- 板子只有一个按钮（与 o3 分析一致）
- 洪哥文档提到的 MODE 按钮（PD3）可能不存在

## 需要确认的问题

1. **PC6 的实际逻辑是什么？**
   - HIGH = 开机，LOW = 关机？
   - 还是 LOW = 开机，HIGH = 关机？

2. **开机时的电源维持机制是什么？**
   - 是否需要 PC6 来维持电源？
   - 还是有其他机制？

3. **PB0 按钮的实际行为？**
   - 按下时是否直接给系统供电？
   - 还是只是触发 MCU 启动？

4. **MODE 按钮是否真的存在？**
   - 照片显示只有一个按钮
   - PD3 是否真的连接了按钮？

## 调试建议

1. 尝试将 PC6 逻辑反转（LOW = 开机）
2. 在开机最早期添加 LED 闪烁，确认 MCU 是否启动
3. 检查按钮是否需要特定的按压时长或模式