# WB32L003K8U6 Project Makefile

# Target
TARGET = firmware

# Debug build?
DEBUG = 1

# Optimization
OPT = -Os

# Build path
BUILD_DIR = build

# LCD variant selection
# Options: lcd_v1_no_offset, lcd_v2_offset_24, lcd_v3_offset_26, lcd_v4_offset_32
VARIANT ?= lcd_v1_no_offset

# Source files
C_SOURCES =  \
Core/Src/main.c \
Core/Src/early_power.c \
Core/Src/st7735.c \
Core/Src/power.c \
Core/Src/ui.c \
Core/Src/ui_assets.c \
Core/Src/hardware_control.c \
Core/Src/audio_adc.c \
Core/Src/syscalls.c \
Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal.c \
Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_cortex.c \
Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_rcc.c \
Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_gpio.c \
Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_spi.c \
Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_adc.c \
Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_tim.c \
Drivers/WB32L003_HAL_Driver/Src/wb32l003_hal_pwr.c \
Drivers/WB32L003_HAL_Driver/Src/system_wb32l003.c

# ASM sources
ASM_SOURCES = startup_wb32l003.s

# Binaries
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
LD = $(PREFIX)ld

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# CPU
CPU = -mcpu=cortex-m0plus

# FPU
FPU = 

# FLOAT-ABI
FLOAT-ABI = -mfloat-abi=soft

# MCU
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# Macros for gcc
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DWB32L003K8U6

# Clock source selection (default: use internal HSI)
# To use external crystal, add USE_HSE=1 to make command
ifeq ($(USE_HSE), 1)
C_DEFS += -DUSE_HSE
else
C_DEFS += -DUSE_HSI_ONLY
endif

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-ICore/Inc \
-IDrivers/WB32L003_HAL_Driver/Inc \
-IDrivers/CMSIS/Include

# Compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -ffreestanding

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# Link script
LDSCRIPT = WB32L003K8Ux_FLASH.ld

# Libraries
LIBS = -L/opt/homebrew/Cellar/arm-none-eabi-gcc/15.1.0/lib/gcc/arm-none-eabi/15.1.0/thumb/v6-m/nofp -L$(BUILD_DIR)/stubs -lgcc -lc -lm -lnosys
LIBDIR = 
LDFLAGS = $(MCU) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -nostartfiles -nodefaultlibs

# Default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# Build the application
# List of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# List of ASM program objects
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
	mkdir -p $(BUILD_DIR)/stubs
	echo "// empty" > $(BUILD_DIR)/stubs/dummy.c
	$(CC) -c $(BUILD_DIR)/stubs/dummy.c -o $(BUILD_DIR)/stubs/dummy.o
	ar rcs $(BUILD_DIR)/stubs/libc.a $(BUILD_DIR)/stubs/dummy.o
	ar rcs $(BUILD_DIR)/stubs/libm.a $(BUILD_DIR)/stubs/dummy.o
	ar rcs $(BUILD_DIR)/stubs/libnosys.a $(BUILD_DIR)/stubs/dummy.o

# Clean up
clean:
	-rm -fR $(BUILD_DIR)

# Dependencies
-include $(wildcard $(BUILD_DIR)/*.d)

# Flash the device
flash: $(BUILD_DIR)/$(TARGET).bin
	@echo "Flashing $(BUILD_DIR)/$(TARGET).bin to device..."
	@echo "Use ST-Link: st-flash write $(BUILD_DIR)/$(TARGET).bin 0x08000000"
	@echo "Use J-Link: JFlash -openprj$(TARGET).jflash -open$(BUILD_DIR)/$(TARGET).bin,0x08000000 -auto -startapp -exit"

# Display size
size: $(BUILD_DIR)/$(TARGET).elf
	@echo "Binary size:"
	@$(SZ) -A -x $(BUILD_DIR)/$(TARGET).elf

.PHONY: all clean flash size