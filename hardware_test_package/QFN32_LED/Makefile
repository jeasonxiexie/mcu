# QFN32_LED Makefile
# Based on verified TIM10_Count project

TARGET = QFN32_LED

# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Paths
CORE_DIR = Core
DRIVERS_DIR = Drivers

# Include paths
INCLUDES = -I$(CORE_DIR)/Inc \
           -I$(DRIVERS_DIR)/CMSIS/Include \
           -I$(DRIVERS_DIR)/CMSIS/Device/WB/WB32L003xx/Include \
           -I$(DRIVERS_DIR)/WB32L003_HAL_Driver/Inc

# Source files
SOURCES = main.c \
          system_wb32l003.c \
          startup_wb32l003.s \
          $(DRIVERS_DIR)/WB32L003_HAL_Driver/Src/wb32l003_gpio.c \
          $(DRIVERS_DIR)/WB32L003_HAL_Driver/Src/wb32l003_rcc.c \
          $(DRIVERS_DIR)/WB32L003_HAL_Driver/Src/wb32l003_tim.c \
          $(DRIVERS_DIR)/WB32L003_HAL_Driver/Src/wb32l003_basetim.c

# Compiler flags
CFLAGS = -mcpu=cortex-m0plus -mthumb -O2 -g -Wall \
         -DWB32L003xx -DUSE_STDPERIPH_DRIVER \
         -ffunction-sections -fdata-sections

# Linker flags
LDFLAGS = -mcpu=cortex-m0plus -mthumb \
          -Tlinker.ld \
          -Wl,--gc-sections -Wl,-Map=$(TARGET).map \
          -lgcc -lc -lnosys

# Build rules
all: $(TARGET).hex

$(TARGET).elf: $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LDFLAGS) -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@
	$(SIZE) $(TARGET).elf

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).map

.PHONY: all clean