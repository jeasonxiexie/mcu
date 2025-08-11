# Makefile for Version 1 - GPIOC Example Schematic
# Standalone build without external dependencies

# Target name
TARGET = lcd_test_v1

# Toolchain
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Source files
SRCS = lcd_example.c \
       lcd_init.c \
       early_power.c \
       system_wb32l003.c \
       hal/wb32l003_hal.c \
       hal/wb32l003_hal_gpio.c \
       hal/wb32l003_hal_rcc.c \
       hal/wb32l003_hal_cortex.c

# Assembly files
ASM_SRCS = startup_wb32l003.s

# Include directories
INCLUDES = -I. \
           -Ihal \
           -Icmsis/Include

# Compiler flags
CFLAGS = -mcpu=cortex-m0plus -mthumb -O2 -g -Wall
CFLAGS += $(INCLUDES)
CFLAGS += -DUSE_HAL_DRIVER -DWB32L003K8U6

# Linker flags
LDFLAGS = -TWB32L003K8Ux_FLASH.ld --gc-sections

# Build directory
BUILD_DIR = build

# Object files
OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
ASM_OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SRCS:.s=.o)))

# Build all
all: $(TARGET).bin $(TARGET).hex

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile C files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: hal/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile assembly files
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create stubs library
$(BUILD_DIR)/stubs.o: stubs.c | $(BUILD_DIR)
	$(CC) -mcpu=cortex-m0plus -mthumb -O2 -c stubs.c -o $@

# Create stubs.c if not exists
stubs.c:
	@echo "// Minimal stubs for missing libc functions" > stubs.c
	@echo "void __libc_init_array(void) {}" >> stubs.c
	@echo "void _exit(int status) { while(1); }" >> stubs.c
	@echo "void *memset(void *s, int c, unsigned int n) {" >> stubs.c
	@echo "    unsigned char *p = s;" >> stubs.c
	@echo "    while(n--) *p++ = c;" >> stubs.c
	@echo "    return s;" >> stubs.c
	@echo "}" >> stubs.c
	@echo "void *memcpy(void *dest, const void *src, unsigned int n) {" >> stubs.c
	@echo "    unsigned char *d = dest;" >> stubs.c
	@echo "    const unsigned char *s = src;" >> stubs.c
	@echo "    while(n--) *d++ = *s++;" >> stubs.c
	@echo "    return dest;" >> stubs.c
	@echo "}" >> stubs.c
	@echo "unsigned int __aeabi_uidiv(unsigned int numerator, unsigned int denominator) {" >> stubs.c
	@echo "    if (denominator == 0) return 0;" >> stubs.c
	@echo "    unsigned int quotient = 0;" >> stubs.c
	@echo "    while (numerator >= denominator) {" >> stubs.c
	@echo "        numerator -= denominator;" >> stubs.c
	@echo "        quotient++;" >> stubs.c
	@echo "    }" >> stubs.c
	@echo "    return quotient;" >> stubs.c
	@echo "}" >> stubs.c
	@echo "void assert_failed(unsigned char *file, unsigned int line) {" >> stubs.c
	@echo "    (void)file;" >> stubs.c
	@echo "    (void)line;" >> stubs.c
	@echo "}" >> stubs.c

# Modify linker script
WB32L003K8Ux_FLASH_nolibc.ld: WB32L003K8Ux_FLASH.ld
	@cp WB32L003K8Ux_FLASH.ld WB32L003K8Ux_FLASH_nolibc.ld
	@sed -i.bak 's/libc\.a ( \* )//' WB32L003K8Ux_FLASH_nolibc.ld
	@sed -i.bak 's/libm\.a ( \* )//' WB32L003K8Ux_FLASH_nolibc.ld
	@sed -i.bak 's/libgcc\.a ( \* )//' WB32L003K8Ux_FLASH_nolibc.ld

# Link
$(TARGET).elf: $(OBJS) $(ASM_OBJS) $(BUILD_DIR)/stubs.o WB32L003K8Ux_FLASH_nolibc.ld
	$(LD) -TWB32L003K8Ux_FLASH_nolibc.ld --gc-sections $(ASM_OBJS) $(OBJS) $(BUILD_DIR)/stubs.o -o $@
	$(SIZE) $@

# Create binary
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Create hex
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# Clean
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET).elf $(TARGET).bin $(TARGET).hex
	rm -f stubs.c WB32L003K8Ux_FLASH_nolibc.ld WB32L003K8Ux_FLASH.ld.bak

.PHONY: all clean