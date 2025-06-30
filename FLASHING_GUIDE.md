# WB32L003 固件烧录指南

## 方法一：使用 J-Link 烧录器（推荐）

### 1. 硬件连接
```
J-Link 烧录器  <->  WB32L003 开发板
SWDIO         <->  SWDIO
SWCLK         <->  SWCLK  
GND           <->  GND
VCC           <->  3.3V (可选，如果板子已供电则不需要)
```

### 2. 软件准备
- 下载并安装 [J-Link Software](https://www.segger.com/downloads/jlink/)
- 确保已安装驱动程序

### 3. 烧录步骤

#### 使用 J-Flash GUI（图形界面）：
1. 打开 J-Flash
2. 创建新项目，选择芯片型号：WB32L003 或 Generic Cortex-M0
3. 设置接口为 SWD，速度 4000 kHz
4. 连接设备 (F9)
5. 打开数据文件：选择 `firmware.bin`
6. 设置起始地址：0x08000000
7. 下载到设备 (F6)
8. 复位运行 (F7)

#### 使用命令行：
```bash
# 进入 J-Link 命令行
JLinkExe -device WB32L003 -if SWD -speed 4000

# 在 J-Link 提示符下执行
connect
loadbin firmware.bin 0x08000000
r
g
exit
```

## 方法二：使用 ST-Link V2 烧录器

### 1. 硬件连接
```
ST-Link V2    <->  WB32L003
SWDIO         <->  SWDIO
SWCLK         <->  SWCLK
GND           <->  GND
3.3V          <->  3.3V (可选)
```

### 2. 使用 STM32 ST-LINK Utility（Windows）
1. 打开 STM32 ST-LINK Utility
2. 连接目标板 (Target -> Connect)
3. 打开固件文件 (File -> Open file -> 选择 firmware.bin)
4. 设置地址：0x08000000
5. 下载 (Target -> Program & Verify)

### 3. 使用 OpenOCD（跨平台）
```bash
# 安装 OpenOCD
brew install openocd  # macOS
# 或 apt-get install openocd  # Linux

# 烧录命令
openocd -f interface/stlink-v2.cfg -f target/stm32f0x.cfg \
-c "program firmware.bin 0x08000000 verify reset exit"
```

## 方法三：使用串口 ISP 烧录

### 1. 硬件连接
```
USB转串口模块  <->  WB32L003
TX            <->  RX (PA10)
RX            <->  TX (PA9)
GND           <->  GND
```

### 2. 进入 ISP 模式
1. 将 BOOT0 引脚拉高（接3.3V）
2. 按复位键或重新上电
3. 此时 MCU 进入串口下载模式

### 3. 使用烧录工具
- Windows: 使用 Flash Loader Demonstrator
- 跨平台: 使用 stm32flash
  ```bash
  # 安装 stm32flash
  brew install stm32flash  # macOS
  
  # 烧录命令
  stm32flash -w firmware.bin -v -g 0x08000000 /dev/tty.usbserial
  ```

## 烧录后验证

1. **LED 指示**：
   - 正常运行时，电源 LED 应该亮起
   - 低电量时，LED 会闪烁

2. **显示屏**：
   - 应该显示开机画面
   - 然后切换到 VU 表显示

3. **按键测试**：
   - 短按：切换 STEREO/MONO 模式
   - 长按3秒：关机

## 常见问题

### 1. 无法连接到目标
- 检查连线是否正确
- 确认目标板已供电
- 尝试降低 SWD 速度到 1000 kHz

### 2. 烧录失败
- 检查芯片是否被锁定
- 确认固件文件路径正确
- 尝试先执行全片擦除

### 3. 烧录后无反应
- 检查 BOOT0 是否已经拉低
- 确认晶振工作正常
- 使用调试器查看程序是否正常运行

## 固件文件位置

最新固件位于：
- `delivery/v2.0/firmware.bin` - 二进制格式
- `delivery/v2.0/firmware.hex` - Intel HEX 格式

两种格式都可以使用，bin 文件需要指定起始地址 0x08000000，hex 文件已包含地址信息。