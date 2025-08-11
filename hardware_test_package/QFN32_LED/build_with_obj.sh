#!/bin/bash

echo "Building with original TIM10_Count object files..."

# Copy original object files
cp ../TIM10_Count/Objects/*.o ./

# Compile our modified main.c
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O2 \
    -DWB32L003xx -DUSE_STDPERIPH_DRIVER \
    -I../TIM10_Count/Libraries/WB32L003x_StdPeriph_Driver/inc \
    -I../TIM10_Count/Libraries/CMSIS/Include \
    -I../TIM10_Count/Libraries/CMSIS/Device/WB/WB32L003x/Include \
    -c main.c -o main_new.o

if [ $? -ne 0 ]; then
    echo "Compilation failed - trying simpler approach"
    
    # Just replace main.o
    echo "Replacing main.o only..."
    rm -f main.o
    cp main_new.o main.o
fi

# Link using original linker script
arm-none-eabi-ld \
    -T ../TIM10_Count/wb32l003x_flash.ld \
    --gc-sections \
    startup_wb32l003.o \
    system_wb32l003.o \
    main.o \
    wb32l003_gpio.o \
    wb32l003_rcc.o \
    wb32l003_tim.o \
    wb32l003_basetim.o \
    -o qfn32_led_final.elf

# Generate hex
arm-none-eabi-objcopy -O ihex qfn32_led_final.elf qfn32_led_final.hex

echo "Generated: qfn32_led_final.hex"
ls -la qfn32_led_final.hex