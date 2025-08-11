#!/bin/bash
# QFN32_LED 直接GCC编译脚本

echo "Compiling QFN32_LED with ARM GCC..."

# 设置编译器
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size

# 编译选项
CFLAGS="-mcpu=cortex-m0plus -mthumb -O2 -g \
        -DWB32L003xx -DUSE_STDPERIPH_DRIVER \
        -ffreestanding -nostdlib \
        -ffunction-sections -fdata-sections"

# 包含路径
INCLUDES="-ICore/Inc \
          -IDrivers/CMSIS/Include \
          -IDrivers/WB32L003_HAL_Driver/Inc"

# 源文件
SOURCES="main.c \
         startup_wb32l003.s \
         system_wb32l003.c \
         Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_gpio.c \
         Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_rcc.c \
         Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_tim.c"

# 检查必要文件
if [ ! -f "main.c" ]; then
    echo "Error: main.c not found!"
    exit 1
fi

if [ ! -f "startup_wb32l003.s" ]; then
    echo "Error: startup_wb32l003.s not found!"
    exit 1
fi

if [ ! -f "system_wb32l003.c" ]; then
    echo "Error: system_wb32l003.c not found!"
    exit 1
fi

# 链接选项
LDFLAGS="-Tlinker.ld \
         -Wl,--gc-sections -Wl,-Map=QFN32_LED.map \
         -lgcc -lc -lnosys"

# 编译
$CC $CFLAGS $INCLUDES $SOURCES $LDFLAGS -o QFN32_LED.elf

if [ $? -eq 0 ]; then
    echo "Creating HEX file..."
    $OBJCOPY -O ihex QFN32_LED.elf QFN32_LED.hex
    
    echo "Build successful!"
    echo ""
    $SIZE QFN32_LED.elf
    echo ""
    echo "Output: QFN32_LED.hex"
else
    echo "Build failed!"
    exit 1
fi