#!/bin/bash

echo "Building QFN32 LED Test (Pure Register Version)..."

# Clean previous builds
rm -f *.o *.elf *.hex *.map

# Compile C source
echo "Compiling qfn32_register.c..."
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -g -O2 \
    -ffreestanding -nostdlib \
    -Wall -ffunction-sections -fdata-sections \
    -c qfn32_register.c -o qfn32_register.o

if [ $? -ne 0 ]; then
    echo "Error compiling C source!"
    exit 1
fi

# Assemble startup code
echo "Assembling startup_minimal.s..."
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -g \
    -c startup_minimal.s -o startup_minimal.o

if [ $? -ne 0 ]; then
    echo "Error assembling startup code!"
    exit 1
fi

# Link
echo "Linking..."
arm-none-eabi-ld \
    -T wb32l003.ld \
    --gc-sections \
    -Map=qfn32_test.map \
    startup_minimal.o qfn32_register.o \
    -o qfn32_test.elf

if [ $? -ne 0 ]; then
    echo "Error linking!"
    exit 1
fi

# Generate hex file
echo "Generating HEX file..."
arm-none-eabi-objcopy -O ihex qfn32_test.elf qfn32_test.hex

if [ $? -ne 0 ]; then
    echo "Error generating HEX!"
    exit 1
fi

# Show size information
echo ""
echo "Build successful! Size information:"
arm-none-eabi-size qfn32_test.elf

# Show hex file info
echo ""
echo "Generated files:"
ls -la qfn32_test.hex
echo ""
echo "HEX file ready: qfn32_test.hex"