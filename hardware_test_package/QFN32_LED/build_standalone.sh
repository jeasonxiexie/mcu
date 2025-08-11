#!/bin/bash

echo "========================================="
echo "构建完全独立的QFN32 LED版本"
echo "不依赖任何外部库文件"
echo "========================================="

# 清理
rm -f *.o *.elf *.hex *.map 2>/dev/null

# 1. 编译C文件
echo "Step 1: 编译 full_standalone.c..."
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O2 \
    -ffreestanding -nostdlib \
    -ffunction-sections -fdata-sections \
    -c full_standalone.c -o full_standalone.o

if [ $? -ne 0 ]; then
    echo "C编译失败!"
    exit 1
fi

# 2. 汇编启动代码
echo "Step 2: 汇编 startup_complete.s..."
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb \
    -c startup_complete.s -o startup_complete.o

if [ $? -ne 0 ]; then
    echo "汇编失败!"
    exit 1
fi

# 3. 链接
echo "Step 3: 链接..."
arm-none-eabi-ld \
    -T wb32l003_complete.ld \
    --gc-sections \
    -Map=qfn32_standalone.map \
    startup_complete.o \
    full_standalone.o \
    -o qfn32_standalone.elf

if [ $? -ne 0 ]; then
    echo "链接失败!"
    exit 1
fi

# 4. 生成hex
echo "Step 4: 生成HEX文件..."
arm-none-eabi-objcopy -O ihex qfn32_standalone.elf qfn32_standalone.hex

if [ $? -eq 0 ]; then
    echo ""
    echo "========================================="
    echo "构建成功!"
    echo "========================================="
    arm-none-eabi-size qfn32_standalone.elf
    echo ""
    ls -la qfn32_standalone.hex
    echo ""
    echo "文件: qfn32_standalone.hex"
    echo ""
    echo "功能说明:"
    echo "  1. PC6: 100kHz PWM输出 (系统关键，CON_POW_CPU)"
    echo "  2. PC0: 红色LED (低电平亮)"
    echo "  3. PC1: 绿色LED (低电平亮)"
    echo "  4. PA3: LCD电源控制 (高电平有效)"
    echo "  5. PC5: RF电源控制 (高电平有效)"
    echo "  6. TIM10中断: LED红绿交替闪烁"
    echo ""
    echo "这个版本包含："
    echo "  - 完整的系统初始化"
    echo "  - SystemInit() 和 SystemCoreClockUpdate()"
    echo "  - 完整的中断向量表（包括TIM10）"
    echo "  - 所有必要的寄存器配置"
else
    echo "生成HEX失败!"
fi