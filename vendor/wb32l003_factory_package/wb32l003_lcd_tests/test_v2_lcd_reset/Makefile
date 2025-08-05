# Simple Makefile template for WB32L003 minimal tests
# 
# Usage: Copy this to each test directory and adjust TARGET name

# Target name (change for each test)
TARGET = test_v2_lcd_reset

# Toolchain
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# MCU specific
MCU = -mcpu=cortex-m0plus -mthumb

# Compiler flags
CFLAGS = $(MCU)
CFLAGS += -Os -g
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -Wextra
CFLAGS += -std=gnu99

# Linker flags
LDFLAGS = $(MCU)
LDFLAGS += -nostdlib
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=$(TARGET).map

# Use minimal startup and linker script
LDFLAGS += -T../startup/wb32l003_minimal.ld

# Source files
SRCS = main.c
OBJS = $(SRCS:.c=.o)
OBJS += ../startup/startup_minimal.o

# Default target
all: $(TARGET).hex

# Compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile assembly files
../startup/%.o: ../startup/%.s
	$(AS) $(MCU) $< -o $@

# Link
$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	$(SIZE) $@

# Create hex file
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).hex $(TARGET).map

.PHONY: all clean