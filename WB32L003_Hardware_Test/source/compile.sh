#!/bin/bash

# 编译脚本 - 生成多个LED版本

echo "编译WB32L003测试程序..."

# 创建简化的启动代码
cat > startup_simple.s << 'EOF'
.syntax unified
.cpu cortex-m0plus
.thumb

.section .vectors, "a"
.word 0x20001000     /* stack top */
.word Reset_Handler  /* reset handler */

.section .text
.thumb_func
.global Reset_Handler
Reset_Handler:
    /* 设置栈指针 */
    ldr r0, =0x20001000
    mov sp, r0
    
    /* 调用early power latch */
    bl early_power_latch
    
    /* 跳转到main */
    bl main
    
    /* 死循环 */
1:  b 1b

.thumb_func
.global early_power_latch
early_power_latch:
    /* RCC->AHBENR |= GPIOCEN */
    ldr r0, =0x40021014
    ldr r1, [r0]
    orr r1, #0x80000
    str r1, [r0]
    
    /* GPIOC->MODER |= (1 << 12) */
    ldr r0, =0x48000800
    ldr r1, [r0]
    orr r1, #0x1000
    str r1, [r0]
    
    /* GPIOC->BSRR = (1 << 6) */
    ldr r0, =0x48000818
    mov r1, #0x40
    str r1, [r0]
    
    bx lr
EOF

# 使用原有的编译环境
cd /Users/jeasonseven/Documents/workspace/BensProject/experiments/lcd_example_test/v2_gpiob_actual_hardware

# PC0/PC1版本
echo "编译 PC0/PC1 版本..."
cp /Users/jeasonseven/Documents/workspace/BensProject/WB32L003_Hardware_Test/source/simple_test.c .
make clean > /dev/null 2>&1
sed -i.bak 's/lcd_test_corrected/simple_test/g' Makefile
make > /dev/null 2>&1
cp lcd_test_v2.hex /Users/jeasonseven/Documents/workspace/BensProject/WB32L003_Hardware_Test/prebuilt/simple_test_pc0_pc1.hex

# 清理
rm simple_test.c
mv Makefile.bak Makefile

echo "完成！生成了 prebuilt/simple_test_pc0_pc1.hex"