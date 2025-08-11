# WB32L003 硬件测试包

## 目的
这是一个独立的测试包，用于验证WB32L003 MCU的基本功能：
- 电源锁存
- LED控制
- 串口输出

## 快速开始

### 1. 检查硬件连接
- 确认MCU型号是 WB32L003
- 确认供电正常（3.3V）
- 准备SWD烧录器

### 2. 修改引脚配置（重要！）
打开 `source/config.h` 文件，根据你的实际硬件修改：

```c
// 电源锁存引脚（最重要）
#define POWER_LATCH_PORT    'C'     // 修改为实际端口
#define POWER_LATCH_PIN     6       // 修改为实际引脚

// LED引脚
#define LED1_PORT           'C'     // 修改为实际端口
#define LED1_PIN            0       // 修改为实际引脚
#define LED2_PORT           'C'     
#define LED2_PIN            1       

// LED极性
#define LED_ACTIVE_LOW      1       // 1=低电平亮, 0=高电平亮
```

### 3. 编译固件

#### Windows系统：
双击 `build.bat` 文件

#### Linux/Mac系统：
```bash
cd source
make
```

### 4. 烧录测试
1. 使用SWD烧录器连接MCU
2. 烧录 `output/test.hex` 文件
3. 复位MCU

### 5. 验证结果

#### 成功的表现：
- LED闪烁（启动时快闪3次，然后每500ms切换）
- 串口输出（PA9, 115200）：
  ```
  ===== WB32L003 Test Program =====
  Power latched on PC6
  LED1 on PC0, LED2 on PC1
  Starting LED flash test...
  Initialization complete!
  [HB] Counter: 0x0004
  [HB] Counter: 0x0008
  ...
  ```

#### 如果不工作：
参考 `TROUBLESHOOTING.md` 文件进行故障排查

## 文件说明

```
hardware_test_package/
├── README.md              # 本文件
├── TROUBLESHOOTING.md     # 故障排查指南
├── source/               
│   ├── config.h          # 【重要】引脚配置文件
│   ├── main.c            # 主程序
│   └── startup.s         # 启动文件
├── build.bat             # Windows编译脚本
├── output/               
│   └── test.hex          # 编译输出
└── docs/                 # 参考文档
```

## 当前默认配置

基于RTF文档的v2版本配置：
- **电源锁存**: PC6 (高电平保持)
- **LED1**: PC0 (低电平点亮)
- **LED2**: PC1 (低电平点亮)
- **串口TX**: PA9 (115200 baud)

## 修改引脚的例子

### 例1：LED在PB0和PB1
```c
#define LED1_PORT           'B'     // 改为B口
#define LED1_PIN            0       // PB0
#define LED2_PORT           'B'     
#define LED2_PIN            1       // PB1
```

### 例2：电源锁存在PB7
```c
#define POWER_LATCH_PORT    'B'     // 改为B口
#define POWER_LATCH_PIN     7       // PB7
```

### 例3：LED高电平点亮
```c
#define LED_ACTIVE_LOW      0       // 改为0
```

## 反馈方式

测试完成后，请提供：
1. 是否看到LED闪烁？
2. 串口输出的内容（截图）
3. 修改后的 `config.h` 文件
4. 测量的电压值（参考TROUBLESHOOTING.md）

## 注意事项

⚠️ **电源锁存必须在上电150ms内完成**，否则MCU会掉电。这是最关键的配置！

⚠️ 如果不确定引脚配置，请先用万用表测量各引脚电压。

## 技术支持

如需帮助，请提供：
- config.h的内容
- 串口输出（如果有）
- 硬件原理图或照片
- 测量的电压值