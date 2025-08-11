# Makefile for QFN32 WB32L003 Test
# Based on working TIM10_Count project

# 工具链配置
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

# CPU配置（Cortex-M0+）
CPU = -mcpu=cortex-m0plus
MCU = $(CPU) -mthumb

# 包含路径 - 使用硬件测试项目的头文件
INC_DIRS = -I../../WB32L003_Hardware_Test/source

# 源文件
SRCS = main.c

# 启动文件
STARTUP = ../source/startup_wb32l003.s

# 编译选项
CFLAGS = $(MCU)
CFLAGS += -Wall -O2
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -std=gnu99
CFLAGS += $(INC_DIRS)
CFLAGS += -DWB32L003xx

# 链接选项
LDFLAGS = $(MCU)
LDFLAGS += -T ../source/wb32l003.ld
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=build/QFN32_Test.map
LDFLAGS += -nostartfiles

# 构建目录
BUILD_DIR = build

# 目标文件
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
OBJS += $(BUILD_DIR)/startup.o

# 默认目标
all: $(BUILD_DIR) $(BUILD_DIR)/QFN32_Test.hex

# 创建构建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 编译C文件
$(BUILD_DIR)/%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# 编译启动文件
$(BUILD_DIR)/startup.o: $(STARTUP)
	@echo "Assembling startup..."
	$(CC) $(MCU) -c $< -o $@

# 链接
$(BUILD_DIR)/QFN32_Test.elf: $(OBJS)
	@echo "Linking..."
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

# 生成HEX文件
$(BUILD_DIR)/QFN32_Test.hex: $(BUILD_DIR)/QFN32_Test.elf
	@echo "Generating $@..."
	$(OBJCOPY) -O ihex $< $@
	@echo "Created: $@"
	@ls -la $@

# 清理
clean:
	rm -rf $(BUILD_DIR)

# 烧录
flash: $(BUILD_DIR)/QFN32_Test.hex
	pyocd flash -t wb32l003 $<

.PHONY: all clean flash