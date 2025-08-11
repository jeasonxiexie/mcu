# WB32L003 QFN32 最小测试固件

## 版本信息
针对QFN32封装优化的最小测试程序，经过专业审查和验证。

## 功能验收标准

### ✅ 必须通过的测试项
1. **LED闪烁**：上电1秒内，红绿LED以0.5秒间隔交替闪烁
2. **串口输出**：38400 8N1，每秒输出心跳信息
3. **电源测量**：PC6/PA3/PC5对地测量约3.3V
4. **不干扰LCD**：未初始化任何LCD/SPI引脚

## 硬件连接

### LED指示
- PC0 (Pin 17) → LED_RED（低电平点亮）
- PC1 (Pin 18) → LED_GREEN（低电平点亮）

### 调试串口
- PB4 (Pin 12) → USB-UART的RX
- GND → GND共地
- 波特率：38400 8N1

### 电源控制（三路必须全部拉高）
- PC6 (Pin 23) → CON_POW_CPU
- PA3 (Pin 7) → CON_POW_LCD  
- PC5 (Pin 22) → CON_POW_RF

## 编译与烧录

```bash
# 编译
make clean
make

# 生成文件
wb32l003_test.hex  # 用于烧录
wb32l003_test.bin  # 二进制格式
```

## 串口输出示例
```
[BOOT] WB32L003 ready @38400 8N1
[INFO] Power: PC6=CPU PA3=LCD PC5=RF
[INFO] LEDs: PC0=RED PC1=GREEN
[INFO] UART: PB4 Software TX
[tick] 1s
[tick] 1s
...
```

## 关键技术特点

1. **SysTick时基**：使用1μs精确时基，确保时序稳定
2. **完整GPIO配置**：MODER/OTYPER/OSPEEDR/PUPDR寄存器全部明确设置
3. **BSRR统一操作**：使用低16位置位、高16位清零，提高可移植性
4. **软件串口稳定性**：38400波特率，基于精确延时实现

## 故障排除

### LED不亮
- 检查PC6/PA3/PC5是否都为3.3V
- 确认LED极性（低电平点亮）
- 验证PC0/PC1焊接

### 串口无输出
- 检查PB4连接到USB-UART的RX
- 确认波特率38400
- GND必须共地

### 系统不稳定
- 确保三路电源都已拉高
- 上电后延时10ms再操作
- 检查晶振32MHz

## 代码大小
```
   text    data     bss     dec     hex filename
    668       0    2056    2724     aa4 wb32l003_test.elf
```

## 注意事项
- PD6已被CHAR占用，不要复用为UART2_RX
- 如果要用硬件UART2，只能用PD5作为TX
- 本版本使用软件串口更稳定可靠