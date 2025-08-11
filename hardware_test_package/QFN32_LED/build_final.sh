#!/bin/bash

echo "Building QFN32 LED using original object files..."

# Method 1: Using hex edit tool (if available)
if command -v hexdump >/dev/null 2>&1; then
    echo "Creating modified hex by patching PC4 to PC0..."
    
    # Copy original hex
    cp ../TIM10_Count/Objects/TIM10_Count.hex qfn32_patched.hex
    
    # This would need actual hex patching which is complex
    echo "Note: Manual hex editing needed to change PC4 references to PC0"
fi

# Method 2: Link with existing object files
echo "Attempting to link with original objects..."

# Use original files
cp ../TIM10_Count/Objects/*.o ./

# Try to extract main.c assembly from main.o
arm-none-eabi-objdump -d main_original.o > main_original.asm

echo "Files prepared. Options:"
echo "1. Use original_working.hex (PC6 PWM works, but uses PC4 not PC0)"
echo "2. Manual hex edit needed to change PC4 to PC0"
echo "3. Need Keil to recompile with modified main.c"

# Show the addresses we need to change
echo ""
echo "In the original code:"
echo "- PC4 toggle is in TIM10_IRQHandler" 
echo "- Need to change GPIO_Pin_4 to GPIO_Pin_0"
echo "- Need to verify PC0 is initialized correctly"

# Create a simple test that just initializes PC0 as output
cat > test_pc0.c << 'EOF'
// Minimal test for PC0 - compile with Keil
#include "wb32l003.h"

int main(void)
{
    SystemCoreClockUpdate();
    
    // Enable GPIOC clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    
    // PC0 as output
    GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_MODE_OUT | GPIO_PUPD_NOPULL | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
    
    // Simple blink
    while(1)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_0);  // LED ON
        for(volatile int i = 0; i < 100000; i++);
        GPIO_SetBits(GPIOC, GPIO_Pin_0);    // LED OFF
        for(volatile int i = 0; i < 100000; i++);
    }
}
EOF

echo "Created test_pc0.c for Keil compilation"