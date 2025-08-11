#!/bin/bash
# QFN32_LED简单编译脚本 - 使用已验证的库文件

echo "Building QFN32_LED..."

# 编译main.c
arm-none-eabi-gcc -c -mcpu=cortex-m0plus -mthumb -O2 -g \
    -DWB32L003xx -DUSE_STDPERIPH_DRIVER \
    -ffreestanding -nostdlib \
    -I. \
    main.c -o main_new.o

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# 链接（使用已有的库文件）
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb \
    -T/Users/jeasonseven/Documents/workspace/BensProject/hardware_test_package/QFN32_LED/linker.ld \
    -Wl,--gc-sections \
    startup_wb32l003.o \
    system_wb32l003.o \
    main_new.o \
    wb32l003_gpio.o \
    wb32l003_rcc.o \
    wb32l003_tim.o \
    wb32l003_basetim.o \
    -lgcc -lc -lnosys \
    -o QFN32_LED.elf

if [ $? -ne 0 ]; then
    echo "Linking failed!"
    exit 1
fi

# 生成HEX文件
arm-none-eabi-objcopy -O ihex QFN32_LED.elf QFN32_LED.hex

echo "Build complete!"
echo "Output: QFN32_LED.hex"
arm-none-eabi-size QFN32_LED.elf