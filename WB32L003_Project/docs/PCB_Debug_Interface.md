# WB32L003 VU表 PCB调试接口说明

## 硬件版本信息
- PCB版本：v1.0（2025年1月）
- MCU型号：WB32L003K8U6 (QFN32封装)
- 调试接口：5针ISP/SWD接口

## 调试接口定义

### 物理布局
```
PCB背面视图（从左到右）：

□ ○ ○ ○ ○
1 2 3 4 5

□ = 方形焊盘（第1脚标识）
○ = 圆形焊盘
```

### 引脚定义

| 孔位 | 信号名称 | 功能说明 | MCU引脚 | 备注 |
|------|----------|----------|---------|------|
| 1 | +3.3V | 电源正极 | VDD | 方形焊盘，丝印标注"+3.3V" |
| 2 | SWDIO | 数据线 | PC7 | Serial Wire Debug数据 |
| 3 | SWCLK | 时钟线 | PD1 | Serial Wire Debug时钟 |
| 4 | NRST | 复位 | NRST | 低电平有效复位 |
| 5 | GND | 电源地 | GND | 最外侧圆形焊盘 |

### 技术规格
- 焊盘间距：1.27mm
- 焊盘类型：通孔设计
- 推荐连接方式：
  - 焊接1.27mm排针
  - 使用弹簧探针（pogo pin）夹具
  - 飞线焊接（调试阶段）

## 烧录器连接

### 支持的烧录器
1. **ST-Link V2**
   ```
   ST-Link引脚  →  PCB引脚
   3.3V         →  1 (+3.3V)
   SWDIO        →  2 (SWDIO/PC7)
   SWCLK        →  3 (SWCLK/PD1)
   RST          →  4 (NRST)
   GND          →  5 (GND)
   ```

2. **J-Link**
   ```
   J-Link引脚   →  PCB引脚
   VTref        →  1 (+3.3V)
   SWDIO/TMS    →  2 (SWDIO/PC7)
   SWCLK/TCK    →  3 (SWCLK/PD1)
   RESET        →  4 (NRST)
   GND          →  5 (GND)
   ```

## 烧录步骤

### Windows系统
1. 使用STM32CubeProgrammer
2. 选择ST-Link连接方式
3. 点击Connect
4. 选择固件文件（.bin或.hex）
5. 起始地址：0x08000000
6. 点击Download

### macOS/Linux系统
```bash
# 使用st-flash工具
st-flash write firmware.bin 0x08000000

# 或使用OpenOCD
openocd -f interface/stlink-v2.cfg -f target/stm32f0x.cfg -c "program firmware.bin 0x08000000 verify reset exit"
```

## 注意事项

### 电源要求
- **必须使用3.3V供电**，不要使用5V
- 烧录时建议断开其他电源（如USB供电）
- 确保烧录器和目标板共地

### 信号完整性
- 连接线缆尽量短（建议<20cm）
- 避免在强干扰环境下烧录
- 如果烧录失败，降低SWD时钟频率

### 常见问题
1. **无法连接**
   - 检查电源是否正常（3.3V）
   - 确认SWDIO/SWCLK连接正确
   - 尝试按住复位键再连接

2. **烧录失败**
   - 确认固件大小不超过64KB
   - 检查起始地址是否为0x08000000
   - 清除Flash后重试

3. **设备无响应**
   - 检查晶振是否工作
   - 确认复位电路正常
   - 使用Connect Under Reset模式

## 量产建议
- 制作专用烧录夹具，使用pogo pin接触
- 考虑在PCB上预留测试点（TP）
- 可以将调试接口改为焊盘设计，减少成本

---
*文档更新日期：2025-01-10*
*作者：Claude Assistant*