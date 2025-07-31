# o3 与 Opus4 原理图分析对比

本文档对比了 o3 和 Opus4 在分析 WB32L003 耳放板原理图时的不同理解。

## 1. LCD/显示屏接口 (最大差异)

### Opus4 的理解：
- 使用硬件 SPI 接口：PC5 (SCK) 和 PC6 (MOSI)
- TFT_CS: PC0
- TFT_DC: PA3  
- TFT_RST: PD3
- 背光: PA1 (PWM控制)

### o3 的理解：
- **LCD 使用 PB7-PB0 连续引脚**，非硬件 SPI
- LCD_RST: PB7
- LCD_RS (DC): PB6
- LCD_SCL: PB5 (软件模拟 SPI 时钟)
- LCD_CS: PB4
- LCD_SDA: PB3 (软件模拟 SPI 数据)
- 背光: PB1 (CLD_BL1) 和 PB2 (CLD_BL2) 两级控制

### 差异原因：
- Opus4 看到 PC5/PC6 标注有 SPI 功能，认为用于 LCD
- o3 发现 PC5/PC6 实际用于电源控制，LCD 连接在 PB 端口

## 2. 电源控制引脚

### Opus4 的理解：
- 未明确指出 PC5/PC6 的电源控制功能
- 认为它们用于 SPI 通信

### o3 的理解：
- **PC6 = CON_POW_CPU**：主电源控制（通过 Q6/Q5 晶体管）
- **PC5 = CON_POW_RF**：5V 升压控制（音频放大器电源）
- PA3 = CON_POW_LCD：LCD 电源控制

## 3. 按键输入

### Opus4 的理解：
- KEY_PWR: PD6
- KEY_MODE: PD4
- 两个独立按键

### o3 的理解：
- **只有一个物理按键 SW_POW**
- SW_POW 检测: **PB0**（不是 PD6）
- PD6 用于充电状态检测（CHAR）
- 没有 MODE 按键，立体声切换由外部滑动开关控制

## 4. 电池电压检测

### Opus4 的理解：
- BAT_ADC: PC0 (ADC_IN15)

### o3 的理解：
- **V_Batt: PC2 (ADC_IN13)**
- 经过 R3/R4 组成的 1:2 分压器

## 5. 充电检测

### Opus4 的理解：
- CHRG: PB5（充电状态）

### o3 的理解：
- **DET: PD7**（5V 检测）
- **CHAR: PD6**（充满检测，来自 TP4056 的 STDBY#）
- PB5 未用于充电检测

## 6. 音频模式控制

### Opus4 的理解：
- MODE_OUT: PB2
- V2 (STEREO/MONO): PB6

### o3 的理解：
- **CON_STEREO: PD3**（控制 U4 模拟开关）
- 没有 MODE_OUT 信号
- PB6 用于 LCD_RS（数据/命令选择）

## 7. 晶体管类型理解

### Opus4 的理解：
- Q5: P-MOSFET

### o3 的理解：
- **Q5: PNP 双极型晶体管（6402）**
- Q6: NPN 晶体管（9014）
- Q7: PNP 晶体管（S8550）用于背光控制

## 8. LCD 型号

### Opus4 的理解：
- TFT096B039

### o3 的理解：
- **LH096NT**
- 连接器型号：ZTB096TB

## 9. 编程接口

### Opus4 的理解：
- 可能支持 SWD 调试

### o3 的理解：
- **只有 ISP 接口**（ISP_CK/ISP_D）
- PC7 用于 ISP 数据线
- 没有标准 SWD 接口

## 10. 未使用引脚

### Opus4 的理解：
- 未特别说明

### o3 的理解：
- PD0, PD4, PD5 未连接
- 部分引脚只标注功能但未实际使用

## 总结

主要差异集中在：
1. **LCD 接口完全不同**：Opus4 认为使用硬件 SPI，o3 发现是 PB 端口的软件 SPI
2. **电源控制理解**：o3 明确了 PC5/PC6 的电源控制功能
3. **按键数量**：Opus4 认为有两个按键，o3 确认只有一个
4. **引脚分配**：多处引脚功能理解不同

o3 的分析更加详细，提供了完整的信号追踪和器件型号确认，应该更接近实际硬件设计。