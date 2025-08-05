# WB32L003 项目架构说明

## 目录结构

```
BensProject/
├── WB32L003_Project/         # 主线固件工程（核心代码）
│   ├── Core/                 # 核心应用代码
│   │   ├── Inc/             # 头文件
│   │   └── Src/             # 源文件（已清理测试代码）
│   ├── Drivers/             # HAL驱动库
│   ├── build/               # 默认构建输出目录
│   ├── delivery/            # 历史发布版本
│   ├── docs/                # 项目文档
│   └── tools/               # 工具脚本
│
├── archive/                  # 历史归档文件
│   ├── 0.96" TFT显示屏.../  # 旧STM32 F103示例
│   └── stm32-clean-template/ # 未使用的模板
│
├── experiments/              # 实验性代码和构建
│   ├── test_sources/        # 各种测试源文件
│   ├── makefiles/           # Makefile变体
│   ├── builds/              # 实验性构建结果
│   └── compile_temp/        # 临时编译测试
│
├── third_party/             # 第三方参考代码
│   └── original_lcd_demos/  # LCD原厂示例代码
│
└── vendor/                  # 厂商协作包
    └── wb32l003_factory_package/  # 给外协的完整包

```

## 引脚映射（重要！）

**所有LCD引脚已修正为GPIOC**（之前错误地使用了GPIOB）：

| LCD信号 | 正确引脚 | GPIO端口 |
|---------|----------|----------|
| SCK     | PC5      | GPIOC    |
| MOSI    | PC6      | GPIOC    |
| CS      | PC0      | GPIOC    |
| RS/DC   | PC1      | GPIOC    |
| RESET   | PC3      | GPIOC    |
| BLK(H)  | PB1      | GPIOB    |
| BLK(L)  | PB2      | GPIOB    |

## 构建方式

### 快速开始
```bash
# 默认构建（无偏移版本）
./build.sh

# 或使用make直接构建
make

# 构建特定LCD变体
make VARIANT=lcd_v2_offset_24
```

### LCD变体选项
- `lcd_v1_no_offset` - 无X轴偏移（默认）
- `lcd_v2_offset_24` - X轴偏移24像素（京东方demo默认）
- `lcd_v3_offset_26` - X轴偏移26像素
- `lcd_v4_offset_32` - X轴偏移32像素

### 清理构建
```bash
make clean
```

## 固件烧录

使用J-Link或WB-Link烧录：
```bash
# 烧录hex文件
./flash.sh build/firmware.hex
```

## 开发说明

1. **主线代码**：所有生产代码都在 `WB32L003_Project/` 下
2. **测试代码**：实验性代码移至 `experiments/test_sources/`
3. **构建输出**：统一输出到 `build/` 目录
4. **版本发布**：使用 `delivery/` 目录记录历史版本

## 注意事项

1. **引脚配置**：确保使用正确的 `pin_config.h`，LCD使用GPIOC而非GPIOB
2. **LCD偏移**：不同LCD面板可能需要不同的X轴偏移，通过VARIANT选择
3. **电源管理**：PC6引脚控制主电源，低电平开启，高电平关闭