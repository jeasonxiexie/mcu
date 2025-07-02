# WB32L003 耳放 VU 表项目

## 项目概述
这是一个基于 WB32L003 MCU 的专业耳放 VU 表项目，具有以下特性：
- ST7735S TFT LCD 驱动 (160x80, SPI 接口)
- 实时音频 VU 表显示（1kHz 采样，32级显示）
- STEREO/MONO 双模式支持
- 电池监测与低电量警告
- 智能电源管理与深度睡眠模式

## 硬件配置（2025-07-01 更新）
- MCU: WB32L003 (Cortex-M0+, 64KB Flash, 4KB SRAM, 24MHz)
- 显示屏: 0.96" TFT ST7735S, 160×80 像素, RGB565
- SPI: SCK=PC5, MOSI=PC6, CS=PB4, DC=PA3, RST=PD3
- 背光: PB1 (TIM1_CH1 PWM)
- ADC输入:
  - 电池电压: PC0 (ADC_IN15, 1/2分压)
  - 左声道: PC1 (ADC_IN14)
  - 右声道: PC2 (ADC_IN13)
- 按键:
  - 电源键: PD6 (长按关机)
  - 模式键: PD4 (STEREO/MONO切换)
- 输出:
  - 红色LED: PD5 (低电量指示)
  - MODE_OUT: PB2 (输出当前模式)
- 调试: UART PB6(TX)/PD6(RX)

## 项目结构
```
WB32L003_Project/
├── Core/
│   ├── Inc/
│   │   ├── pin_config.h       # 引脚定义（新）
│   │   ├── board_config.h     # 板级配置（新）
│   │   ├── audio_adc.h        # 音频ADC接口
│   │   ├── hardware_control.h # 硬件控制接口
│   │   ├── st7735.h          # 显示驱动
│   │   ├── power.h           # 电源管理
│   │   ├── ui.h              # 用户界面
│   │   └── wb32l003.h        # MCU定义
│   └── Src/
│       ├── main.c            # 主程序（已更新）
│       ├── audio_adc.c       # 音频采集（已更新）
│       ├── hardware_control.c # 硬件控制（已更新）
│       ├── st7735.c          # 显示驱动
│       ├── power.c           # 电源管理
│       └── ui.c              # 界面实现
├── Drivers/
│   ├── CMSIS/
│   └── WB32L003_HAL_Driver/
├── startup_wb32l003.s
├── WB32L003K8Ux_FLASH.ld
├── Makefile
└── build.sh
```

## Building the Project

### Prerequisites
Install the ARM embedded toolchain:
- macOS: `brew install arm-none-eabi-gcc`
- Linux: `sudo apt-get install gcc-arm-none-eabi`
- Windows: Download from ARM website

### Build Steps
```bash
# Make build script executable
chmod +x build.sh

# Build the project
./build.sh
```

This will generate:
- `build/WB32L003_Project.elf` - Debug file
- `build/WB32L003_Project.hex` - Intel HEX format
- `build/WB32L003_Project.bin` - Binary for flashing

## Flashing the Firmware

### Using ST-Link
```bash
chmod +x flash_stlink.sh
./flash_stlink.sh
```

### Using J-Link
```bash
chmod +x flash_jlink.sh
./flash_jlink.sh
```

## 功能实现

### 显示系统
- 支持 RGB565 格式图像
- 硬件 SPI 12MHz 高速更新
- PWM 控制背光（4档亮度）
- 实时 VU 表音频可视化

### 音频系统
- 3通道 ADC 同步采样（电池+左右声道）
- 1kHz 采样率，TIM3 触发
- 32级音频电平显示
- STEREO/MONO 模式切换

### 电源管理
- 电池电压监测（1/2分压）
- 4级电量指示
- 低电量警告（<3.0V 红灯闪烁）
- 临界电量自动关机（<2.8V）
- 深度睡眠模式（~10μA）

### 用户界面
- 开机/关机动画
- VU 表动态显示
- 模式状态指示
- 背光自动调节

## 内存使用
- Flash: ~32 KB / 64 KB (50%)
- RAM: ~2.5 KB / 4 KB (62.5%)

## 开发说明
- 使用 HSI 24MHz 内部时钟
- Flash 0 等待周期
- SWD 调试接口：PC7(SWDIO), PD1(SWDCLK)
- 预留 Flash 末页(0xF800)用于设置存储

## 版本历史
- v2.0.0 (2025-07-01): 更新为新的 WB32L003 引脚映射
- v1.0.0 (2025-06-30): 初始版本

## 许可证
本项目为 WB32L003 开发板专用固件。