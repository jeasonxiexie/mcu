# Firmware Version 3.2.0 - Hardware Team Verification Fix

## Release Date: 2025-07-31

## Overview
This firmware version implements fixes based on 洪哥's hardware team documentation, correcting critical power control logic and adding MODE button support.

## Major Changes from v3.0.x

### 1. **Fixed Power Control Logic** (CRITICAL FIX)
- PC6 = HIGH for ON, LOW for OFF (inverted from previous version)
- This was the main reason why v3.0.x didn't power on properly

### 2. **Added MODE Button Support**
- PD3 is MODE button (KEY_STEREO) for stereo/mono switching
- PD2 controls stereo/mono output (not PD3)
- Power button remains on PB0

### 3. **Updated Backlight Control**
- PB1 = LOW to enable backlight power via Q7
- PB2 = HIGH (bright), LOW (dim), High-Z (off)
- Added 20-second auto-dim timeout (TODO)

### 4. **Corrected Power Sequences**
- Power ON: PC6 HIGH → Mute → PA3 HIGH → LCD init → PB1 LOW → PB2 HIGH → PC5 HIGH → Unmute
- Power OFF: Mute → PC5 LOW → PB2 High-Z → PB1 High-Z → PA3 LOW → PC6 LOW

### 5. **Fixed Pin Assignments**
- CON_STEREO moved from PD3 to PD2
- All power control pins now use correct logic levels

## Technical Details

### Power Control Pins (Verified by Hardware Team)
- PC6 (CON_POW_CPU): HIGH = ON, LOW = OFF
- PC5 (CON_POW_RF): HIGH = ON, LOW = OFF  
- PA3 (CON_POW_LCD): HIGH = ON, LOW = OFF

### User Interface
- PB0: Power button (long press for power off)
- PD3: MODE button (stereo/mono switching)
- PC0: Red LED (LOW = ON)
- PC1: Green LED (LOW = ON)

### LCD Interface (Unchanged)
- Software SPI on PB7-PB0
- PB5 (SCL), PB3 (SDA), PB4 (CS), PB6 (RS), PB7 (RST)

## Version History
- v3.2.0: Initial release based on hardware team documentation
- v3.0.x: Based on o3 analysis (had inverted power control)
- v2.x: Earlier versions

## Files Included
- `firmware_v3.2.0.hex` - Intel HEX format for programming
- `firmware_v3.2.0.bin` - Binary format

## Programming Instructions
Use WB-Link with ISP interface (PC7 for clock/data) to program the firmware.
Base address: 0x00000000