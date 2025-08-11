# Version 1 - Pin Mapping (Standard Schematic)
## 基于主板原理图的标准设计

### LCD接口引脚 (SPI)
| 功能 | MCU引脚 | GPIO端口 | 说明 |
|------|---------|----------|------|
| LCD_SCL | PC5 | GPIOC | SPI时钟信号 |
| LCD_SDA | PC6 | GPIOC | SPI数据信号(MOSI) |
| LCD_CS | PC0 | GPIOC | 片选信号 |
| LCD_DC | PC1 | GPIOC | 数据/命令选择 |
| LCD_RES | PC3 | GPIOC | 复位信号 |

### 背光控制
| 功能 | MCU引脚 | GPIO端口 | 说明 |
|------|---------|----------|------|
| LCD_BL1 | PB1 | GPIOB | 背光控制1 (低电平有效) |
| LCD_BL2 | PB2 | GPIOB | 背光控制2 (直接驱动) |

### 电源控制
| 功能 | MCU引脚 | GPIO端口 | 说明 |
|------|---------|----------|------|
| LCD_PWR | PA3 | GPIOA | LCD电源使能 |
| LCD_VCC | - | - | 3.3V供电 |
| LCD_GND | - | - | 地线 |

### 特点
- 使用**GPIOC**端口作为主要LCD接口
- 软件SPI实现
- 基于主板原理图的标准设计
- 适用于标准0.96寸插接式LCD模块开发测试

### 注意事项
⚠️ 此版本基于主板原理图设计，如果使用实际PCB板请使用V2版本