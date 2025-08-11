# WB32L003 固件测试方案总结

## 问题分析

TIM10_Count成功的关键在于：
1. 使用官方HAL库，包含完整的初始化序列
2. 正确配置GPIO的所有寄存器（MODER, OTYPER, OSPEEDR, PUPDR, AFR）
3. 使用硬件定时器生成可靠的PWM信号
4. 正确的系统时钟初始化

我们之前的固件缺失：
- OSPEEDR（速度寄存器）配置
- PUPDR（上拉/下拉寄存器）配置
- 系统时钟初始化不完整

## 解决方案

创建了两个测试版本：

### 1. test_hal.hex（HAL库方式）
- 文件：`build_test/test_hal.hex`（2747字节）
- 特点：模拟TIM10_Count的GPIO_Init函数
- 包含完整的寄存器配置序列
- 可选择使用TIM1生成PWM（像TIM10_Count一样）

### 2. test_reg.hex（改进的寄存器版本）
- 文件：`build_test/test_reg.hex`（2546字节）
- 特点：直接寄存器操作，但包含所有必要配置
- 添加了之前缺失的OSPEEDR和PUPDR配置
- 完整的系统时钟初始化

## 测试步骤

### 第一步：烧录固件
```bash
# 烧录HAL版本
pyocd flash -t wb32l003 build_test/test_hal.hex

# 或者烧录寄存器版本
pyocd flash -t wb32l003 build_test/test_reg.hex
```

### 第二步：使用示波器验证
1. **PC6引脚**（最关键）
   - 应该输出稳定的高电平（3.3V）
   - 这是CPU电源控制，必须保持高电平
   
2. **PC5引脚**
   - 应该输出高电平（RF电源）
   
3. **PA3引脚**
   - 应该输出高电平（LCD电源）
   
4. **PC0/PC1引脚**（LED）
   - 应该每500ms切换一次
   - 启动时快闪3次

### 第三步：预期结果
- 如果PC6输出高电平 → GPIO初始化正确
- 如果LED闪烁 → 程序正常运行
- 如果所有电源引脚都是高电平 → 电源管理正常

## 关键改进

1. **系统时钟初始化**
```c
// 启用HSI并等待稳定
RCC->CR |= 0x01;
while(!(RCC->CR & 0x02));
```

2. **完整的GPIO配置**
```c
// 包含所有寄存器
GPIOx->MODER   // 模式
GPIOx->OTYPER  // 输出类型
GPIOx->OSPEEDR // 速度（新增）
GPIOx->PUPDR   // 上拉/下拉（新增）
```

3. **正确的配置顺序**
- 先初始化时钟
- 然后配置GPIO
- 最后设置输出电平

## 如果还是不工作

1. 检查Flash起始地址是否正确（应该是0x00000000）
2. 确认晶振频率（代码假设12MHz）
3. 考虑使用TIM1硬件PWM（取消test_hal.c中Tim1_Config()的注释）

## 文件列表

```
build_test/
├── test_hal.hex  # HAL库版本（推荐）
├── test_reg.hex  # 寄存器版本
├── test_hal.elf  # 调试用ELF文件
├── test_reg.elf  # 调试用ELF文件
└── *.map         # 链接器映射文件

源代码：
├── test_hal.c    # HAL库版本源码
├── test_reg.c    # 寄存器版本源码
└── Makefile.test # 构建脚本
```

## 下一步

1. 先烧录test_hal.hex（更可靠）
2. 用示波器测PC6
3. 如果PC6有高电平，LED应该也会闪烁
4. 如果还不行，可能需要检查硬件连接或晶振配置