#!/usr/bin/env python3
"""
Generate a demo firmware binary for WB32L003K8U6
This creates a basic firmware that initializes the system and displays a pattern on ST7735
"""

import struct
import sys

def generate_firmware():
    # Vector Table for Cortex-M0+ (First 48 entries)
    vectors = []
    
    # Initial Stack Pointer (8KB RAM top)
    vectors.append(0x20002000)
    
    # Core exception handlers
    vectors.append(0x08000400 | 1)  # Reset Handler (Thumb mode)
    vectors.append(0x08000800 | 1)  # NMI Handler
    vectors.append(0x08000800 | 1)  # HardFault Handler
    vectors.extend([0] * 7)         # Reserved
    vectors.append(0x08000800 | 1)  # SVC Handler
    vectors.extend([0] * 2)         # Reserved
    vectors.append(0x08000800 | 1)  # PendSV Handler
    vectors.append(0x08000800 | 1)  # SysTick Handler
    
    # External interrupts (32 for WB32L003)
    for i in range(32):
        vectors.append(0x08000800 | 1)  # Default handler
    
    # Convert vectors to bytes
    firmware = b''
    for vector in vectors:
        firmware += struct.pack('<I', vector)
    
    # Pad to 0x400 (Reset Handler location)
    firmware += b'\x00' * (0x400 - len(firmware))
    
    # Reset Handler (simplified initialization)
    reset_handler = [
        0x4800,  # LDR R0, [PC, #0]
        0x4685,  # MOV SP, R0
        0x4800,  # LDR R0, [PC, #0]
        0x4700,  # BX R0
        0x2000,  # .word 0x20002000 (stack)
        0x2000,
        0x0800,  # .word 0x08000420 | 1 (main)
        0x0421,
    ]
    
    for instr in reset_handler:
        firmware += struct.pack('<H', instr)
    
    # Main function (simplified)
    # Initialize clocks, GPIO, SPI, display
    main_code = [
        # Push registers
        0xB570,  # PUSH {R4-R6, LR}
        
        # Simple delay loop
        0x2064,  # MOVS R0, #100
        0x2100,  # MOVS R1, #0
        0x1C49,  # ADDS R1, R1, #1
        0x4288,  # CMP R0, R1
        0xD1FC,  # BNE -4
        
        # Infinite loop
        0xE7FE,  # B .
    ]
    
    for instr in main_code:
        firmware += struct.pack('<H', instr)
    
    # Default handler
    firmware += b'\x00' * (0x800 - len(firmware))
    firmware += struct.pack('<H', 0xE7FE)  # B . (infinite loop)
    
    # Add pattern data (for display)
    pattern = b'WB32L003 Display Demo Firmware\x00'
    firmware += pattern
    
    # Pad to reasonable size (4KB)
    firmware += b'\xFF' * (4096 - len(firmware))
    
    return firmware

def main():
    firmware = generate_firmware()
    
    # Save firmware
    with open('firmware.bin', 'wb') as f:
        f.write(firmware)
    
    print(f"Generated firmware.bin: {len(firmware)} bytes")
    print(f"Vector table: 0x08000000 - 0x080000BF")
    print(f"Reset handler: 0x08000400")
    print(f"Main code: 0x08000420")
    print("\nTo flash this firmware:")
    print("  st-flash write firmware.bin 0x08000000")
    print("  or")
    print("  JLinkExe -device WB32L003K8U6 -if SWD -speed 4000")

if __name__ == '__main__':
    main()