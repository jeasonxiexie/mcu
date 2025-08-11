# Makefile for Linux/Mac compilation
TARGET = test
CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CFLAGS = -mcpu=cortex-m0plus -mthumb -O2 -g -Wall -fdata-sections -ffunction-sections
LDFLAGS = -TWB32L003K8Ux_FLASH_nolibc.ld --gc-sections

SRCS = main.c stubs.c system_wb32l003.c
ASMS = startup_wb32l003.s
OBJS = $(SRCS:.c=.o) $(ASMS:.s=.o)

all: $(TARGET).hex $(TARGET).bin
	$(SIZE) $(TARGET).elf
	@echo "Build complete: $(TARGET).hex"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.o *.elf *.hex *.bin *.map

.PHONY: all clean