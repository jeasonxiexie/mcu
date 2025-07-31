# Firmware Version 3.3.0 - o3 Transistor Logic Fix

## Release Date: 2025-07-31

## Overview
This version implements the correct power control logic based on o3's analysis of the actual transistor circuits. The main issue was that all power control pins use inverted logic due to P-MOS and PNP transistors.

## Critical Fixes from v3.2.x

### 1. **PC6 Power Control - INVERTED LOGIC** ✅
- **Correct**: PC6 = LOW to keep power ON (P-MOS transistor)
- **Wrong (v3.2)**: PC6 = HIGH to keep power ON
- This was the root cause of power-on failure

### 2. **PA3 LCD Power - INVERTED LOGIC** ✅  
- **Correct**: PA3 = LOW to enable LCD power (PNP transistor)
- **Wrong (v3.2)**: PA3 = HIGH to enable LCD power

### 3. **PB1 Backlight Control** ✅
- Must set PB1 = LOW first to enable backlight power via Q7 PNP
- Then PB2 controls brightness (HIGH = bright, LOW = dim)

## Technical Details (from o3 analysis)

### Power Control Circuit Analysis
| Signal | Transistor | Type | Logic |
|--------|------------|------|-------|
| PC6 (CON_POW_CPU) | Q5 via Q6 | P-MOS | LOW = ON, HIGH = OFF |
| PA3 (CON_POW_LCD) | Q7 | PNP | LOW = ON, HIGH = OFF |
| PB1 (CLD_BL1) | Q7 | PNP | LOW = ON, HIGH = OFF |

### Why the Logic is Inverted
- **Q5 (P-MOS)**: Gate needs to be LOW relative to source to conduct
- **Q7 (PNP)**: Base needs to be LOW relative to emitter to conduct
- 洪哥's RTF document had the logic backwards

## Changes in This Version

1. **main.c**: Set PC6 LOW immediately on startup
2. **hardware_control.c**: Fixed all power control functions
3. **pin_config.h**: Updated comments with correct logic

## Test Instructions

1. Burn `firmware_v3.3.0.hex` to the device
2. Insert USB-C for power
3. Press and hold the power button for 1-2 seconds
4. Release the button - device should stay powered on
5. Green LED should light up if battery voltage > 3.0V

## Version History
- v3.3.0: Fixed transistor logic based on o3 analysis
- v3.2.x: Based on incorrect hardware team documentation
- v3.0.x: Based on initial o3 analysis (had correct PC6 logic!)

## Files
- `firmware_v3.3.0.hex` - Intel HEX format
- `firmware_v3.3.0.bin` - Binary format