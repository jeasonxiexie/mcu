# WB32L003 硬件测试程序

## 项目说明

这是一个用于测试 WB32L003 MCU 基本功能的最简化程序。主要用于验证：
1. MCU能否正常启动
2. 电源锁存是否工作
3. LED引脚配置是否正确
4. 串口通信是否正常

## 快速开始

### 方法1：使用预编译的HEX文件

在 `prebuilt/` 目录下有几个预编译版本：
- `simple_test_pc0_pc1.hex` - LED在PC0/PC1（默认）
- `simple_test_pb0_pb1.hex` - LED在PB0/PB1
- `simple_test_pa0_pa1.hex` - LED在PA0/PA1

直接烧录对应的HEX文件到MCU即可。

### 方法2：修改配置后重新编译

1. 打开 `source/simple_test.c`
2. 修改配置区域的引脚定义：

```c
// LED配置 - 根据实际硬件修改！
#define LED1_GPIO           'C'      // 改为实际GPIO组
#define LED1_PIN            0         // 改为实际引脚号
#define LED2_GPIO           'C'      
#define LED2_PIN            1         

// LED极性配置
#define LED_ACTIVE_LOW      1         // 1=低电平亮, 0=高电平亮
```

3. 编译方法：

#### Windows命令行编译（推荐）
双击运行 `build.bat`

#### Keil编译
1. 打开 `keil_project/simple_test.uvprojx`
2. 点击 Build 或按 F7

#### 手动GCC编译
```bash
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O2 source/simple_test.c source/startup.s -T source/linker.ld -o simple_test.elf
arm-none-eabi-objcopy -O ihex simple_test.elf simple_test.hex
```

## 测试步骤

1. **烧录程序**
   - 使用SWD烧录器将HEX文件烧录到MCU
   - 烧录后断电重启

2. **观察现象**
   - LED应该先快闪3次（启动成功标志）
   - 然后LED1和LED2交替闪烁（主循环运行）

3. **串口调试**（可选）
   - 连接PA9到USB转串口的RX
   - 波特率：115200, 8N1
   - 应该看到启动信息和定期的心跳消息

## 故障排查

如果LED不亮，请按以下步骤排查：

### 1. 检查电源锁存
- 测量PC6电压，应该是3.3V
- 如果是0V，说明电源锁存失败
- 检查是否在150ms内执行了锁存代码

### 2. 确认LED引脚
- 用万用表测量配置的LED引脚
- 应该看到电压在0V和3.3V之间切换
- 如果电压不变，可能是引脚配置错误

### 3. 检查LED极性
- 如果引脚电压正常但LED不亮
- 尝试修改 `LED_ACTIVE_LOW` 的值
- 1表示低电平亮，0表示高电平亮

### 4. 验证MCU运行
- 最好的方法是查看串口输出
- 如果有串口输出，说明MCU正在运行
- 如果没有，可能是MCU未启动或卡死

## 引脚配置说明

查看 `PIN_CONFIGURATION.txt` 了解当前的引脚配置。

## 需要帮助？

如果测试不成功，请提供以下信息：
1. 实际的LED引脚连接（哪个GPIO的哪个引脚）
2. LED是高电平亮还是低电平亮
3. 串口是否有输出
4. PC6的电压测量值
5. LED引脚的电压变化情况

## 文件结构

```
WB32L003_Hardware_Test/
├── README.md                # 本文件
├── TROUBLESHOOTING.md       # 详细故障排查指南
├── PIN_CONFIGURATION.txt    # 引脚配置说明
├── source/                  # 源代码
│   └── simple_test.c       # 可配置的测试程序
├── prebuilt/               # 预编译版本
│   ├── simple_test_pc0_pc1.hex
│   ├── simple_test_pb0_pb1.hex
│   └── simple_test_pa0_pa1.hex
├── build.bat               # Windows编译脚本
└── docs/                   # 参考文档
```