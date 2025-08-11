#!/bin/bash

echo "Building QFN32 Power-First Version..."

# Clean
rm -f *.o power_first.elf power_first.hex

# Compile
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -g -O2 \
    -ffreestanding -nostdlib \
    -Wall -ffunction-sections -fdata-sections \
    -c qfn32_power_first.c -o power_first.o

# Assemble startup
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -g \
    -c startup_minimal.s -o startup_minimal.o

# Link
arm-none-eabi-ld \
    -T wb32l003.ld \
    --gc-sections \
    -Map=power_first.map \
    startup_minimal.o power_first.o \
    -o power_first.elf

# Generate hex
arm-none-eabi-objcopy -O ihex power_first.elf power_first.hex

echo "Generated: power_first.hex"
arm-none-eabi-size power_first.elf