# LCD Example Test Makefile for WB32L003K8U6

# Target
TARGET = lcd_example

# Build directory
BUILD_DIR = build

# C sources
C_SOURCES = \
lcd_example.c \
lcd_init.c \
../../WB32L003_Project/Core/Src/early_power.c \
../../WB32L003_Project/Core/Src/syscalls.c \
../../WB32L003_Project/Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal.c \
../../WB32L003_Project/Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_cortex.c \
../../WB32L003_Project/Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_rcc.c \
../../WB32L003_Project/Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_gpio.c \
../../WB32L003_Project/Drivers/WB32L003_HAL_Driver/Src/system_wb32l003.c

# ASM sources
ASM_SOURCES = \
../../WB32L003_Project/startup_wb32l003.s

# Binaries
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# CPU
CPU = -mcpu=cortex-m0plus

# FPU
FPU = 

# FLOAT-ABI
FLOAT_ABI = -mfloat-abi=soft

# MCU
MCU = $(CPU) -mthumb $(FPU) $(FLOAT_ABI)

# Optimization
OPT = -Os

# C defines
C_DEFS = \
-DUSE_HAL_DRIVER \
-DWB32L003K8U6 \
-DUSE_HSI_ONLY

# C includes
C_INCLUDES = \
-I. \
-I../../WB32L003_Project/Core/Inc \
-I../../WB32L003_Project/Drivers/WB32L003_HAL_Driver/Inc \
-I../../WB32L003_Project/Drivers/CMSIS/Include

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# Linker script
LDSCRIPT = ../../WB32L003_Project/WB32L003K8Ux_FLASH.ld

# Libraries
LIBS = -lc -lm -lnosys
LIBDIR = -L$(BUILD_DIR)/stubs
LDFLAGS = $(MCU) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -nostartfiles

# Default action: build all
all: $(BUILD_DIR) $(BUILD_DIR)/stubs/libc.a $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# Create stub libraries
$(BUILD_DIR)/stubs/libc.a: | $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/stubs
	echo "// empty" > $(BUILD_DIR)/stubs/dummy.c
	$(CC) -c $(BUILD_DIR)/stubs/dummy.c -o $(BUILD_DIR)/stubs/dummy.o
	ar rcs $(BUILD_DIR)/stubs/libc.a $(BUILD_DIR)/stubs/dummy.o
	ar rcs $(BUILD_DIR)/stubs/libm.a $(BUILD_DIR)/stubs/dummy.o
	ar rcs $(BUILD_DIR)/stubs/libnosys.a $(BUILD_DIR)/stubs/dummy.o

# Build the application
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

# Clean up
clean:
	-rm -fR $(BUILD_DIR)

# Display size
size: $(BUILD_DIR)/$(TARGET).elf
	@echo "Firmware size:"
	@$(SZ) $(BUILD_DIR)/$(TARGET).elf

# Dependencies
-include $(wildcard $(BUILD_DIR)/*.d)

# Phony targets
.PHONY: all clean size