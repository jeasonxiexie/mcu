#!/bin/bash

# Direct linking without standard libraries
echo "Building firmware.elf..."

arm-none-eabi-ld \
    build/main.o \
    build/st7735.o \
    build/power.o \
    build/ui.o \
    build/syscalls.o \
    build/wb32l003_hal.o \
    build/wb32l003_hal_cortex.o \
    build/wb32l003_hal_rcc.o \
    build/wb32l003_hal_gpio.o \
    build/wb32l003_hal_spi.o \
    build/wb32l003_hal_adc.o \
    build/wb32l003_hal_tim.o \
    build/wb32l003_hal_pwr.o \
    build/system_wb32l003.o \
    build/startup_wb32l003.o \
    -L/opt/homebrew/Cellar/arm-none-eabi-gcc/15.1.0/lib/gcc/arm-none-eabi/15.1.0/thumb/v6-m/nofp \
    -Lbuild/stubs \
    -lgcc \
    -lc \
    -lm \
    -lnosys \
    -T WB32L003K8Ux_FLASH.ld \
    -Map=build/firmware.map \
    --gc-sections \
    -o build/firmware.elf

if [ $? -eq 0 ]; then
    echo "Building firmware.hex..."
    arm-none-eabi-objcopy -O ihex build/firmware.elf build/firmware.hex
    
    echo "Building firmware.bin..."
    arm-none-eabi-objcopy -O binary -S build/firmware.elf build/firmware.bin
    
    echo "Firmware size:"
    arm-none-eabi-size build/firmware.elf
    
    echo "Build complete!"
else
    echo "Build failed!"
fi