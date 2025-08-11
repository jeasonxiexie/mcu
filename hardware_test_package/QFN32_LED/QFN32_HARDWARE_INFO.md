# QFN32 硬件信息汇总

## MCU基本信息
- **型号**: WB32L003 (ARM Cortex-M0+)
- **封装**: QFN32
- **Flash**: 32KB (起始地址: 0x00000000)
- **RAM**: 4KB (起始地址: 0x20000000)
- **主频**: 12MHz (系统复位后默认)

## QFN32引脚功能映射（最终版）

### LED / 指示
| 引脚 | QFN32 Pin | 功能 | 说明 |
|------|-----------|------|------|
| PC0  | Pin 17    | LED_RED | 低电平亮 |
| PC1  | Pin 18    | LED_GREEN | 低电平亮 |

### 调试串口
| 引脚 | QFN32 Pin | 功能 | 说明 |
|------|-----------|------|------|
| PD5  | Pin 30    | UART2_TX | 推荐接USB-UART的RX，单向日志 |
| PD6  | Pin 31    | UART2_RX | 已占用为CHAR充满检测，别接 |

### 电源控制
| 引脚 | QFN32 Pin | 功能 | 说明 |
|------|-----------|------|------|
| PC6  | Pin 23    | CON_POW_CPU | 开机=HIGH，**系统关键100kHz PWM** |
| PA3  | Pin 7     | CON_POW_LCD | 开机=HIGH |
| PC5  | Pin 22    | CON_POW_RF | 开机=HIGH |

### 按键
| 引脚 | QFN32 Pin | 功能 | 说明 |
|------|-----------|------|------|
| PB0  | Pin 16    | SW_POW | LOW=开机 |
| PD3  | Pin 28    | KEY_STEREO (MODE) | 模式切换键 |

### 音频与静音/立体声
| 引脚 | QFN32 Pin | 功能 | 说明 |
|------|-----------|------|------|
| PA4  | Pin 8     | CON_MUTE | HIGH=解除静音 |
| PD2  | Pin 27    | CON_STEREO | LOW=立体声 / HIGH=单声道 |

### ADC
| 引脚 | QFN32 Pin | 功能 | 说明 |
|------|-----------|------|------|
| PC2  | Pin 19    | 电池电压 | 1:2分压 |
| PC4  | Pin 21    | 左声道 | ADC输入 |
| PC3  | Pin 20    | 右声道 | ADC输入 |

### 充电检测
| 引脚 | QFN32 Pin | 功能 | 说明 |
|------|-----------|------|------|
| PD7  | Pin 32    | 5V DET | 5V检测 |
| PD6  | Pin 31    | CHAR | 充满检测（与UART2_RX复用，优先当检测脚）|

### LCD接口（暂未使用）
| 引脚 | QFN32 Pin | 功能 | 说明 |
|------|-----------|------|------|
| PB7  | Pin 9     | LCD_RESET | 复位信号 |
| PB6  | Pin 10    | LCD_RS/DC | 数据/命令选择 |
| PB5  | Pin 11    | LCD_SCL | SPI时钟 |
| PB4  | Pin 12    | LCD_CS | 片选 |
| PB3  | Pin 13    | LCD_SDA | SPI数据 |
| PB2  | Pin 14    | LCD_BL2 | 背光控制2 |
| PB1  | Pin 15    | LCD_BL1 | 背光控制1 |

## 硬件验证历史

### 已验证功能
1. **PC6 100kHz PWM** ✅
   - 硬件工程师已测试
   - 使用TIM10_Count原始固件
   - 输出稳定的100kHz方波

2. **MCU基本功能** ✅
   - Flash烧写正常
   - 时钟系统正常
   - 定时器工作正常

### 待验证功能
1. **GPIO输出** ❓
   - PC0/PC1 LED控制
   - PA3/PC5 电源控制

2. **GPIO输入** ❓
   - 按键输入等

## 测试固件版本

### 原始版本
- **文件**: TIM10_Count.hex
- **功能**: PC6输出100kHz PWM
- **状态**: ✅ 已验证工作

### 修改版本
1. **qfn32_test.hex** (纯寄存器版本)
   - PC6: 100kHz PWM (保持不变)
   - PA3: LCD电源HIGH
   - PC5: RF电源HIGH
   - PC0/PC1: LED交替闪烁
   - **特点**: 无库依赖，纯寄存器操作

2. **main.c** (标准库版本)
   - 基于原项目修改
   - 需要Keil编译

3. **main_optimized.c** (优化版本)
   - 整理后的代码结构
   - 需要Keil编译

## 编译环境

### ARM GCC编译
```bash
# 成功的编译命令
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O2 \
    -ffreestanding -nostdlib \
    -c qfn32_register.c

arm-none-eabi-ld -T wb32l003.ld \
    --gc-sections \
    startup_minimal.o qfn32_register.o \
    -o qfn32_test.elf

arm-none-eabi-objcopy -O ihex qfn32_test.elf qfn32_test.hex
```

### 库兼容性问题
- **标准外设库**: GPIO_Init, RCC_AHBPeriphClockCmd (原项目使用)
- **HAL库**: HAL_GPIO_Init, __HAL_RCC_GPIOC_CLK_ENABLE (不兼容)
- **解决方案**: 使用纯寄存器操作避免库依赖

## 寄存器地址映射

### 重要基地址
```c
#define RCC_BASE        0x40021000
#define GPIOA_BASE      0x48000000  
#define GPIOC_BASE      0x48000800
#define GPIOD_BASE      0x48000C00
#define TIM1_BASE       0x40012C00
#define TIM10_BASE      0x40014400
```

### GPIO寄存器偏移
```c
MODER   +0x00  // 模式寄存器
OTYPER  +0x04  // 输出类型
OSPEEDR +0x08  // 速度寄存器
PUPDR   +0x0C  // 上下拉
BSRR    +0x18  // 置位/复位
AFR[0]  +0x20  // 复用功能低
AFR[1]  +0x24  // 复用功能高
```

## 关键发现

1. **PC6是CPU电源控制（CON_POW_CPU）**
   - 必须保持100kHz PWM输出
   - 开机=HIGH，这是系统工作的基础
   - 连接到电源管理芯片
   - 绝对不能改为普通GPIO

2. **Flash起始地址是0x00000000**
   - 不是STM32的0x08000000
   - 链接脚本必须正确配置

3. **引脚复用冲突**
   - PD6同时是UART2_RX和CHAR充满检测
   - 优先用作充满检测，不要接串口

4. **完整的音频VU计设计**
   - PC2/PC3/PC4: ADC采集音频和电池
   - PA4: 静音控制
   - PD2: 立体声/单声道切换
   - 完整的LCD接口（PB1-PB7）

## 下一步计划

1. 测试qfn32_test.hex验证GPIO功能
2. 如果GPIO不工作，改用PWM方式控制LED
3. 逐步添加其他外设功能（SPI、I2C等）

## 联系信息
- 硬件工程师：[待补充]
- 测试日期：2024-08-11
- 项目状态：GPIO功能验证中