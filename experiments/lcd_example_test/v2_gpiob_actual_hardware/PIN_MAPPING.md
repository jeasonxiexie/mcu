# Version 2 - Pin Mapping (Actual PCB)
## 基于实际PCB板的引脚定义

### LCD接口引脚 (SPI)
| 功能 | MCU引脚 | GPIO端口 | 说明 |
|------|---------|----------|------|
| LCD_SCL | PB5 | GPIOB | SPI时钟信号 |
| LCD_SDA | PB3 | GPIOB | SPI数据信号(MOSI) |
| LCD_CS | PB4 | GPIOB | 片选信号 |
| LCD_RS | PB6 | GPIOB | 数据/命令选择 (DC) |
| LCD_RESET | PB7 | GPIOB | 复位信号 |

### 背光控制
| 功能 | MCU引脚 | GPIO端口 | 说明 |
|------|---------|----------|------|
| CLD_BL1 | PB1 | GPIOB | 背光控制-高亮 (通过Q7 PNP晶体管) |
| CLD_BL2 | PB2 | GPIOB | 背光控制-低亮/开关 |

**背光控制逻辑**:
- PB1 LOW = Q7导通，给LCD背光供电
- PB2 HIGH = 高亮显示，LOW = 开机

### 电源控制
| 功能 | MCU引脚 | GPIO端口 | 说明 |
|------|---------|----------|------|
| CON_POW_LCD | PA3 | GPIOA | LCD电源控制 (HIGH=开) |
| CON_POW_CPU | PC6 | GPIOC | CPU主电源控制 (HIGH=开/保持) |
| CON_POW_RF | PC5 | GPIOC | 5V升压控制 |

### 用户接口
| 功能 | MCU引脚 | GPIO端口 | 说明 |
|------|---------|----------|------|
| SW_POW | PB0 | GPIOB | 电源开关 |
| LED_RED | PC0 | GPIOC | 红色LED (LOW=点亮) |
| LED_GREEN | PC1 | GPIOC | 绿色LED (LOW=点亮) |

### 音频控制
| 功能 | MCU引脚 | GPIO端口 | 说明 |
|------|---------|----------|------|
| CON_MUTE | PA4 | GPIOA | 静音控制 (LOW=静音) |

### 特点
- 使用**GPIOB**端口作为主要LCD接口
- 软件SPI实现
- 基于实际PCB板的引脚定义
- 包含完整的电源管理和背光控制

### 注意事项
⚠️ 此版本是实际PCB板的引脚定义
⚠️ 与例程版本不同，LCD接口使用GPIOB而非GPIOC