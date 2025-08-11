#!/bin/bash

echo "Creating complete hex using original system files..."

# Use all original object files except main.o
cp ../TIM10_Count/Objects/startup_wb32l003.o ./
cp ../TIM10_Count/Objects/system_wb32l003.o ./
cp ../TIM10_Count/Objects/wb32l003_*.o ./

# Create minimal main that just changes PC4 to PC0
cat > main_pc0.c << 'EOF'
volatile unsigned int* GPIOC_BSRR = (volatile unsigned int*)0x48000818;

void TIM10_IRQHandler(void)
{
    // Clear interrupt flag (simplified)
    volatile unsigned int* TIM10_SR = (volatile unsigned int*)0x40014410;
    *TIM10_SR = 0;
    
    // Toggle PC0 instead of PC4
    static int toggle = 0;
    if(toggle) {
        *GPIOC_BSRR = (1 << 0);  // PC0 LOW
    } else {
        *GPIOC_BSRR = (1 << 16); // PC0 HIGH
    }
    toggle = !toggle;
}
EOF

# Compile just the interrupt handler
arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O2 -c main_pc0.c -o main_pc0.o

# Extract the original main.o but remove TIM10_IRQHandler
arm-none-eabi-objcopy -N TIM10_IRQHandler main_original.o main_no_tim10.o 2>/dev/null || echo "Objcopy attempt"

# Try to link everything
echo "Attempting link..."
arm-none-eabi-ld \
    -T ../TIM10_Count/Objects/TIM10_Count.sct \
    startup_wb32l003.o \
    system_wb32l003.o \
    main_original.o \
    main_pc0.o \
    wb32l003_gpio.o \
    wb32l003_rcc.o \
    wb32l003_tim.o \
    wb32l003_basetim.o \
    -o qfn32_merged.elf 2>&1 | head -10

# If link fails, try simpler approach
echo "Trying hex merge approach..."

# Convert original hex to binary
arm-none-eabi-objcopy -I ihex -O binary original_working.hex original.bin

# Check size
ls -la original.bin

echo "Note: Need to patch binary at TIM10_IRQHandler location"
echo "PC4 = 0x10 (bit 4), PC0 = 0x01 (bit 0)"