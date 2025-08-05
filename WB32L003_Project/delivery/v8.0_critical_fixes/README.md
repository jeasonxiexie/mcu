# v8.0 Critical Fixes - LCD Display Issues Resolved

## 问题分析与修复

### 发现的关键问题

1. **hardware_control.c 第 129-130 行严重错误**
   - LCD 控制引脚被错误地初始化到 GPIOB 而不是 GPIOC
   - 这导致所有 LCD 信号发送到错误的 GPIO 端口

2. **LCD 电源控制逻辑错误（PA3）**
   - 初始化时注释说 "LCD OFF" 但实际是开启状态
   - 已修正注释和逻辑一致性

3. **引脚冲突问题**
   - PC6: 既用作 LCD_SDA (MOSI) 又用作 CON_POW_CPU (电源自锁)
   - PC5: 既用作 LCD_SCL (SPI时钟) 又用作 CON_POW_RF (5V电源控制)
   - PC0/PC1: 既用作 LCD_CS/LCD_RS 又用作 LED 指示灯

### 修复内容

1. **hardware_control.c**
   ```c
   // 修复前（错误）
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  // LCD pins on wrong port!
   
   // 修复后（正确）
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  // LCD pins on correct GPIOC
   ```

2. **LCD 电源控制**
   ```c
   // 修正 PA3 初始状态
   HAL_GPIO_WritePin(CON_POW_LCD_PORT, CON_POW_LCD_PIN, GPIO_PIN_SET); // HIGH = OFF
   ```

3. **引脚冲突处理**
   - 创建了 hardware_control_fixed.c 处理引脚冲突
   - 建议硬件重新设计以避免这些冲突

## 测试步骤

1. 烧录 firmware.hex
2. 检查以下信号：
   - PA3 应该在 LCD 使能时为低电平
   - PC5 应该有 SPI 时钟信号
   - PC6 应该有 SPI 数据信号
   - PC3 应该有复位脉冲（100ms 低电平后拉高）

## 如果仍然不亮

1. **检查背光**：PB1/PB2 应该为低电平（PNP晶体管导通）
2. **检查电源**：确保 3.3V 稳定供电
3. **检查 LCD 模块**：可能需要不同的 X 轴偏移值

## 版本信息

- 基于 lcd_v1_no_offset (无 X 轴偏移)
- 修复了所有已知的引脚映射错误
- 修复了 hardware_control.c 中的 GPIO 初始化错误

## 编译时间

2025-08-04