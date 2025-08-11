#!/bin/bash

echo "构建完整版本 - 使用原项目的所有系统文件"
echo "==========================================="

# 1. 复制所有原始的系统文件
echo "Step 1: 复制原始系统文件..."
cp ../TIM10_Count/Objects/startup_wb32l003.o ./
cp ../TIM10_Count/Objects/system_wb32l003.o ./
cp ../TIM10_Count/Objects/wb32l003_gpio.o ./
cp ../TIM10_Count/Objects/wb32l003_rcc.o ./
cp ../TIM10_Count/Objects/wb32l003_tim.o ./
cp ../TIM10_Count/Objects/wb32l003_basetim.o ./

# 2. 创建修改版的main（只改PC4为PC0）
echo "Step 2: 创建兼容的main..."
cat > main_compatible.s << 'EOF'
    .syntax unified
    .cpu cortex-m0plus
    .thumb

    .global TIM10_IRQHandler
    .section .text.TIM10_IRQHandler
    .thumb_func
TIM10_IRQHandler:
    push {r4, lr}
    
    @ Check TIM10 flag
    ldr r0, =0x40014410  @ TIM10->SR
    ldr r1, [r0]
    tst r1, #1
    beq .L_exit
    
    @ Clear flag
    movs r1, #0
    str r1, [r0]
    
    @ Toggle PC0 (not PC4)
    ldr r0, =0x48000818  @ GPIOC->BSRR
    ldr r1, =toggle_state
    ldr r2, [r1]
    cmp r2, #0
    beq .L_set_high
    
.L_set_low:
    movs r3, #1      @ Set PC0 low (LED on)
    str r3, [r0]
    movs r2, #0
    str r2, [r1]
    b .L_exit
    
.L_set_high:
    movs r3, #1
    lsls r3, #16     @ Set PC0 high (LED off)
    str r3, [r0]
    movs r2, #1
    str r2, [r1]
    
.L_exit:
    pop {r4, pc}

    .section .data
toggle_state:
    .word 0
EOF

# 3. 汇编
echo "Step 3: 汇编新的中断处理..."
arm-none-eabi-as -mcpu=cortex-m0plus -mthumb main_compatible.s -o main_mod.o

# 4. 从原main.o中提取其他函数（除了TIM10_IRQHandler）
echo "Step 4: 处理原main.o..."
# 这一步比较复杂，我们用另一种方法

# 5. 创建简单的链接脚本
echo "Step 5: 创建链接脚本..."
cat > link_complete.ld << 'EOF'
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 32K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 4K
}

ENTRY(Reset_Handler)

SECTIONS
{
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } >FLASH

    .text :
    {
        *(.text*)
        *(.rodata*)
    } >FLASH

    .data :
    {
        _sdata = .;
        *(.data*)
        _edata = .;
    } >RAM AT>FLASH

    .bss :
    {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } >RAM

    _estack = ORIGIN(RAM) + LENGTH(RAM);
}
EOF

# 6. 尝试链接
echo "Step 6: 链接..."
arm-none-eabi-ld \
    -T link_complete.ld \
    --gc-sections \
    startup_wb32l003.o \
    system_wb32l003.o \
    ../TIM10_Count/Objects/main.o \
    main_mod.o \
    wb32l003_gpio.o \
    wb32l003_rcc.o \
    wb32l003_tim.o \
    wb32l003_basetim.o \
    -o qfn32_complete.elf 2>&1 | head -5

# 如果成功，生成hex
if [ -f qfn32_complete.elf ]; then
    arm-none-eabi-objcopy -O ihex qfn32_complete.elf qfn32_complete.hex
    echo "成功! 生成: qfn32_complete.hex"
    ls -la qfn32_complete.hex
else
    echo "链接失败 - 需要其他方法"
fi

echo ""
echo "备选方案："
echo "1. 找一个Windows电脑用Keil编译"
echo "2. 用Wine运行Keil"
echo "3. 找到完整的WB32L003标准外设库源码"