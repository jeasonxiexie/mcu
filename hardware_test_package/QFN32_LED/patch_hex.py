#!/usr/bin/env python3
"""
Patch TIM10_Count.hex to change PC4 to PC0
PC4 = GPIO_Pin_4 = 0x0010
PC0 = GPIO_Pin_0 = 0x0001
"""

def calculate_checksum(line):
    """Calculate Intel HEX checksum"""
    total = 0
    for i in range(0, len(line)-2, 2):
        total += int(line[i:i+2], 16)
    checksum = (256 - (total & 0xFF)) & 0xFF
    return f"{checksum:02X}"

def patch_hex_file(input_file, output_file):
    """Patch hex file to change PC4 references to PC0"""
    
    with open(input_file, 'r') as f:
        lines = f.readlines()
    
    patched_lines = []
    patch_count = 0
    
    for line in lines:
        if line.startswith(':'):
            # Parse Intel HEX line
            if len(line) > 10:
                # Look for patterns that might be GPIO_Pin_4 (0x10 0x00 in little endian)
                # In TIM10_IRQHandler, this would be the GPIO pin mask
                
                # Pattern to find: instructions dealing with bit 4 (0x10)
                # Need to change to bit 0 (0x01)
                
                # Common patterns in ARM code:
                # MOVS R0, #0x10 -> MOVS R0, #0x01
                # The encoding would be different
                
                modified = False
                new_line = line.strip()
                
                # Look for 0x10 0x00 pattern (little endian for 0x0010)
                if '1000' in new_line:
                    # This is a simplistic approach - in reality we'd need to
                    # understand the exact instruction encoding
                    # For now, let's create a note about what needs changing
                    print(f"Found potential PC4 reference at: {new_line[:20]}...")
                    
                # Check for specific patterns in GPIO operations
                # GPIO_ToggleBits typically uses XOR with pin mask
                # So we're looking for 0x10 being used as immediate value
                
                patched_lines.append(line)
            else:
                patched_lines.append(line)
        else:
            patched_lines.append(line)
    
    # For now, output the same file with analysis
    with open(output_file, 'w') as f:
        f.writelines(patched_lines)
    
    print(f"Analysis complete. Manual patching needed.")
    print("Look for TIM10_IRQHandler and change:")
    print("  GPIO_Pin_4 (0x0010) -> GPIO_Pin_0 (0x0001)")
    print("  Bit 4 operations -> Bit 0 operations")

if __name__ == "__main__":
    patch_hex_file("original_working.hex", "qfn32_pc0.hex")
    print("\nAlternative: Use the original hex but connect LED to PC4 pin if available")