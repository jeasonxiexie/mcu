#!/bin/bash

echo "========================================="
echo "构建完整的QFN32 LED版本"
echo "使用原始系统文件 + 完整初始化"
echo "========================================="

# 清理
rm -f *.o *.elf *.hex *.map 2>/dev/null

# 1. 复制原始的关键系统文件
echo "Step 1: 复制原始系统文件..."
cp ../TIM10_Count/Objects/startup_wb32l003.o ./
cp ../TIM10_Count/Objects/system_wb32l003.o ./
cp ../TIM10_Count/Objects/wb32l003_gpio.o ./
cp ../TIM10_Count/Objects/wb32l003_rcc.o ./
cp ../TIM10_Count/Objects/wb32l003_tim.o ./
cp ../TIM10_Count/Objects/wb32l003_basetim.o ./

# 2. 编译我们的完整main
echo "Step 2: 编译main_complete.c..."
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O2 \
    -ffreestanding -nostdlib \
    -ffunction-sections -fdata-sections \
    -c main_complete.c -o main_complete.o

if [ $? -ne 0 ]; then
    echo "编译失败!"
    exit 1
fi

# 3. 创建包含所有段的链接脚本
echo "Step 3: 创建链接脚本..."
cat > wb32l003_complete.ld << 'EOF'
/* Memory configuration for WB32L003 */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 32K
    RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 4K
}

/* Entry point */
ENTRY(Reset_Handler)

/* Stack size */
_Min_Heap_Size = 0x100;
_Min_Stack_Size = 0x400;

SECTIONS
{
    /* Interrupt vectors */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    /* Program code */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)
        
        KEEP(*(.init))
        KEEP(*(.fini))
        
        . = ALIGN(4);
        _etext = .;
    } >FLASH

    /* ARM unwinding */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH

    .ARM :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    .preinit_array :
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array*))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } >FLASH
    
    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        PROVIDE_HIDDEN(__init_array_end = .);
    } >FLASH
    
    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array*))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } >FLASH

    /* Used by startup to initialize data */
    _sidata = LOADADDR(.data);

    /* Initialized data */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT>FLASH

    /* Uninitialized data */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM

    /* User heap and stack */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE(end = .);
        PROVIDE(_end = .);
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } >RAM

    /* Remove debug information */
    /DISCARD/ :
    {
        *(.ARM.attributes)
        *(.comment)
        *(.debug*)
    }
    
    /* Stack pointer */
    _estack = ORIGIN(RAM) + LENGTH(RAM);
}
EOF

# 4. 链接
echo "Step 4: 链接所有文件..."
arm-none-eabi-ld \
    -T wb32l003_complete.ld \
    --gc-sections \
    -Map=qfn32_final.map \
    startup_wb32l003.o \
    system_wb32l003.o \
    main_complete.o \
    wb32l003_gpio.o \
    wb32l003_rcc.o \
    wb32l003_tim.o \
    wb32l003_basetim.o \
    -o qfn32_final.elf

if [ $? -ne 0 ]; then
    echo "链接失败!"
    
    # 尝试更简单的方法
    echo "尝试备用链接方法..."
    arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb \
        -T wb32l003_complete.ld \
        -nostdlib -nostartfiles \
        -Wl,--gc-sections \
        startup_wb32l003.o \
        system_wb32l003.o \
        main_complete.o \
        -o qfn32_final.elf 2>&1 | head -10
fi

# 5. 生成hex
if [ -f qfn32_final.elf ]; then
    echo "Step 5: 生成HEX文件..."
    arm-none-eabi-objcopy -O ihex qfn32_final.elf qfn32_final.hex
    
    echo ""
    echo "========================================="
    echo "构建成功!"
    echo "========================================="
    arm-none-eabi-size qfn32_final.elf
    echo ""
    ls -la qfn32_final.hex
    echo ""
    echo "文件: qfn32_final.hex"
    echo "功能:"
    echo "  - PC6: 100kHz PWM (系统关键)"
    echo "  - PC0/PC1: LED红绿交替闪烁"
    echo "  - PA3/PC5: 电源控制输出高"
else
    echo "构建失败 - 需要完整的库文件"
fi