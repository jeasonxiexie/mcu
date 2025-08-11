# WB32L003 QFN32 测试固件 Makefile

# 工具链
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# 目标名称
TARGET = wb32l003_test

# 源文件
SRCS = src/main.c
ASRCS = startup_wb32l003.s

# 目标文件
OBJS = $(SRCS:.c=.o) $(ASRCS:.s=.o)

# 编译选项
CFLAGS = -mcpu=cortex-m0 -mthumb
CFLAGS += -Os -ffunction-sections -fdata-sections
CFLAGS += -Wall -Wextra
CFLAGS += -Iinc

# 汇编选项
ASFLAGS = -mcpu=cortex-m0 -mthumb

# 链接选项
LDFLAGS = -T wb32l003.ld
LDFLAGS += -mcpu=cortex-m0 -mthumb
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -nostdlib -nodefaultlibs -nostartfiles
LDFLAGS += -Wl,-Map=$(TARGET).map

# 默认目标
all: $(TARGET).hex $(TARGET).bin size

# 编译C文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 编译汇编文件
%.o: %.s
	$(CC) $(ASFLAGS) -c $< -o $@

# 链接
$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# 生成HEX文件
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# 生成BIN文件
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# 显示大小
size: $(TARGET).elf
	$(SIZE) $<

# 清理
clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).map

# 烧录（需要根据实际烧录器调整）
flash: $(TARGET).hex
	@echo "请使用合适的烧录工具烧录 $(TARGET).hex"

.PHONY: all clean size flash