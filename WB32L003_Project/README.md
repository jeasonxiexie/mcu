# WB32L003 耳放项目

基于 WB32L003 MCU 的 0.96" TFT 显示屏驱动项目。

## 快速开始

```bash
# 克隆仓库
git clone <repository-url>
cd BensProject/WB32L003_Project

# 构建固件
./build.sh

# 或指定LCD变体
make VARIANT=lcd_v2_offset_24
```

## 硬件信息

- **MCU**: WB32L003 (ARM Cortex-M0+)
- **显示屏**: 0.96" TFT (ST7735S, 80x160)
- **接口**: 软件SPI

### 引脚映射（已修正）

| 功能 | 引脚 | 说明 |
|------|------|------|
| LCD_SCK | PC5 | SPI时钟 |
| LCD_MOSI | PC6 | SPI数据 |
| LCD_CS | PC0 | 片选 |
| LCD_RS | PC1 | 数据/命令 |
| LCD_RST | PC3 | 复位 |
| LCD_BL1 | PB1 | 背光控制 |

## 构建选项

支持多种LCD偏移配置：
- `lcd_v1_no_offset` - 无偏移（默认）
- `lcd_v2_offset_24` - 京东方标准偏移
- `lcd_v3_offset_26` - 26像素偏移
- `lcd_v4_offset_32` - 32像素偏移

详见 [README_VARIANTS.md](README_VARIANTS.md)

## 项目结构

- `Core/` - 核心应用代码
- `Drivers/` - HAL驱动库
- `build/` - 构建输出
- `delivery/` - 发布版本
- `docs/` - 项目文档

其他目录说明见 [docs/architecture.md](docs/architecture.md)

## 烧录方法

使用J-Link或WB-Link烧录hex文件：
```bash
# 示例
JLinkExe -device WB32L003K8U6 -if SWD -speed 4000 -CommandFile flash.jlink
```

## 故障排查

1. **屏幕不亮**：检查引脚连接，确保LCD连接到GPIOC
2. **显示偏移**：尝试不同的VARIANT选项
3. **编译错误**：确保安装了arm-none-eabi-gcc工具链

## 许可证

本项目仅供学习和参考使用。