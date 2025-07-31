#!/usr/bin/env python3
"""
Generate a power test sequence that tries ALL possible combinations
of power control pins to help debug why the device won't power on.
"""

import itertools

# Power control pins and their possible states
pins = {
    'PC6': ['LOW', 'HIGH'],   # Main power control
    'PA3': ['LOW', 'HIGH'],   # LCD power control  
    'PB1': ['LOW', 'HIGH'],   # Backlight enable
    'PB2': ['LOW', 'HIGH'],   # Backlight brightness
}

# Generate all combinations
all_combinations = list(itertools.product(*pins.values()))

print(f"Total combinations to test: {len(all_combinations)}\n")

# Print each combination with analysis
for i, combo in enumerate(all_combinations):
    pc6, pa3, pb1, pb2 = combo
    print(f"Test {i+1}:")
    print(f"  PC6 = {pc6:4} (Main power: {'ON' if pc6 == 'LOW' else 'OFF'} per o3)")
    print(f"  PA3 = {pa3:4} (LCD power: {'ON' if pa3 == 'LOW' else 'OFF'} per o3)")
    print(f"  PB1 = {pb1:4} (Backlight: {'ON' if pb1 == 'LOW' else 'OFF'} per o3)")
    print(f"  PB2 = {pb2:4} (Brightness: {'Bright' if pb2 == 'HIGH' else 'Dim'})")
    
    # Highlight potentially working combinations
    if pc6 == 'LOW':  # Power should be ON
        if pa3 == 'LOW' and pb1 == 'LOW':
            print("  ⭐ This should work according to o3's analysis!")
        elif pa3 == 'HIGH' and pb1 == 'HIGH':
            print("  ⚠️  This matches 洪哥's document (but o3 says it's wrong)")
    
    print()

print("\nAdditional tests to consider:")
print("1. Pulse PC6 (edge triggered power?)")
print("2. Test PD0/PD1 pins (undocumented power control?)")
print("3. Hold power button (PB0) while testing")
print("4. Try different power-up sequences (PC6 first vs PA3 first)")
print("5. Check if we need pull-up/pull-down resistors")