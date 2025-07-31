# Changelog - v3.5.0

## 问题根源（o3 分析）

### 死循环原因
1. 用户按下 PB0 按钮 → 通过 D300/D302 拉低 P-MOS 栅极 → 电源开启
2. 按钮只能维持约 **150ms** 的低电平
3. 如果 MCU 在 150ms 内没有把 PC6 拉低 → 电源关闭 → MCU 掉电
4. MCU 重启 → 重复步骤 1 → **死循环**

### 为什么 v3.3.0 失败
虽然逻辑正确（PC6=LOW 开启电源），但设置时机太晚：
- Reset → SystemInit → HAL_Init() → 设置 PC6
- 整个过程已经超过 150ms！

## 修复方案

### 1. 创建早期电源锁存函数
```c
// Core/Src/early_power.c
void Early_PowerLatch(void) {
    RCC->AHBENR |= (1UL << 19);     // 开启 GPIOC 时钟
    GPIOC->MODER &= ~(3UL << 12);   // PC6 设为输出
    GPIOC->MODER |= (1UL << 12);
    GPIOC->BRR = (1UL << 6);        // PC6 = LOW！
}
```

### 2. 修改启动流程
```asm
; startup_wb32l003.s
Reset_Handler:
    ldr   r0, =_estack
    mov   sp, r0
    
    ; 关键：立即锁住电源！
    bl    Early_PowerLatch
    
    ; 然后才是正常启动流程...
```

### 3. 清理 main.c
移除了原来在 main() 中的 PC6 设置代码，因为已经在更早的地方处理了。

## 测试验证

### 必须的操作
1. **按住按钮 2 秒以上** - 给 MCU 足够时间启动
2. 松开按钮后，设备应该保持开机
3. 屏幕背光应该亮起

### 如果失败
- 按钮时间不够长
- 硬件损坏
- 进入了 ISP 模式

## 技术细节

- 使用直接寄存器操作，不依赖 HAL
- 在栈初始化后立即执行
- 最小化代码路径，确保快速执行
- 时钟启用后只用 4 个 NOP 延时

## 文件变更

1. **新增**: `Core/Src/early_power.c`
2. **修改**: `startup_wb32l003.s` - 添加早期调用
3. **修改**: `Core/Src/main.c` - 移除冗余代码
4. **修改**: `Makefile` - 添加 early_power.c