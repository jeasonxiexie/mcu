# LCD驱动测试记录

搞了个0.96寸LCD(ST7735S)接在WB32L003上，一直点不亮。发现原理图和实际板子的引脚可能不一样。

## 文件结构

```
lcd_example_test/
├── v1_gpioc_standard_schematic/   # 版本1 - 基于标准原理图 (GPIOC)
├── v2_gpiob_actual_hardware/      # 版本2 - 基于实际硬件 (GPIOB)  
├── firmware_output/                # 编译输出目录 (自动生成)
├── build_all.sh                    # Unix/macOS 一键编译脚本
└── build_all.bat                   # Windows 一键编译脚本
```

## 问题记录

按照例程和主板原理图写的代码，LCD接口用的GPIOC(PC0/1/3/5/6)，但是不亮。

找到个RTF文档(WB32L003 耳放固件.rtf)，里面写着：
```
LCD_RST -> PB7
LCD_RS  -> PB6  
LCD_SCL -> PB5
LCD_CS  -> PB4
LCD_SDA -> PB3
```

引脚定义完全不一样，所以整理了两个版本：
- v1_gpioc_standard_schematic/ - 按原理图
- v2_gpiob_actual_hardware/ - 按RTF文档

## 编译环境

arm-none-eabi-gcc (我用的15.1.0版本)

## 编译方法

Windows下运行:
```bash
build_all.bat
```

Linux/Mac下运行:
```bash
./build_all.sh
```

编译好的固件在 `firmware_output/` 里面，文件名带时间戳。

## 单独编译

如果只想编译其中一个版本，进到对应目录运行build脚本就行了。


## 引脚定义差异

| 功能 | 原理图版本(GPIOC) | 实际板子(GPIOB) |
|------|------------------|----------------|
| SDA | PC6 | PB3 |
| SCL | PC5 | PB5 |
| CS | PC0 | PB4 |
| DC/RS | PC1 | PB6 |
| RST | PC3 | PB7 |
| 背光 | PB1/PB2 | PB1/PB2 |


MCU: WB32L003K8U6
LCD: 0.96寸 ST7735S