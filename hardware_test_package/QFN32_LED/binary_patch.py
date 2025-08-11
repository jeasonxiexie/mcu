#!/usr/bin/env python3
"""
Binary patch to change PC4 to PC0 in the original hex
GPIO_Pin_4 = 0x0010 (bit 4)
GPIO_Pin_0 = 0x0001 (bit 0)
"""

import sys

def patch_hex_file():
    # Read original hex
    with open('original_working.hex', 'r') as f:
        lines = f.readlines()
    
    # Convert to binary
    import subprocess
    subprocess.run(['arm-none-eabi-objcopy', '-I', 'ihex', '-O', 'binary', 
                    'original_working.hex', 'original.bin'])
    
    # Read binary
    with open('original.bin', 'rb') as f:
        data = bytearray(f.read())
    
    print(f"Binary size: {len(data)} bytes")
    
    # In ARM Thumb code, GPIO_Pin_4 (0x10) might appear as:
    # MOVS Rx, #0x10  -> encoded as 20 10
    # LDR Rx, =0x10   -> stored in literal pool
    
    # Search for patterns
    patches = []
    
    # Look for 0x10 0x00 pattern (little endian 0x0010)
    for i in range(len(data) - 1):
        if data[i] == 0x10 and data[i+1] == 0x00:
            # Check context to see if this might be GPIO_Pin_4
            if i > 100 and i < len(data) - 100:
                # Look for nearby GPIO-related code
                # GPIOC base is 0x48000800
                nearby = data[i-50:i+50]
                if b'\x00\x08\x00\x48' in nearby or b'\x48\x00\x08\x00' in nearby:
                    print(f"Potential GPIO_Pin_4 at offset 0x{i:04x}")
                    patches.append(i)
    
    # Conservative patch - only patch the most likely location
    # In TIM10_IRQHandler, GPIO_ToggleBits uses the pin mask
    
    if len(patches) > 0:
        print(f"Found {len(patches)} potential patch locations")
        
        # Create patched version
        patched = bytearray(data)
        
        # Patch the most likely one (usually in the second half of the binary)
        for offset in patches:
            if offset > len(data) // 2:  # More likely to be in interrupt handler
                print(f"Patching offset 0x{offset:04x}: 0x10 -> 0x01")
                patched[offset] = 0x01  # Change to GPIO_Pin_0
                break
        
        # Write patched binary
        with open('patched.bin', 'wb') as f:
            f.write(patched)
        
        # Convert back to hex
        subprocess.run(['arm-none-eabi-objcopy', '-I', 'binary', '-O', 'ihex',
                       '--set-start', '0x00000000',
                       'patched.bin', 'qfn32_pc0_patched.hex'])
        
        print("Created: qfn32_pc0_patched.hex")
        print("WARNING: This is experimental! Test carefully.")
    else:
        print("Could not find clear patch location")
        print("Manual analysis needed")

if __name__ == "__main__":
    patch_hex_file()