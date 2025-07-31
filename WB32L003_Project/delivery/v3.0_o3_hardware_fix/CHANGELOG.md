# Firmware Version 3.0.1 - o3 Hardware Verification Fix

## Release Date: 2025-07-31
## Updated: 2025-07-31 (v3.0.1 - Fixed Flash base address)

## Overview
This firmware version implements comprehensive hardware fixes based on o3's detailed schematic analysis and verification.

## Major Changes

### Pin Configuration Updates (based on o3 verification)
- **LCD Interface**: Switched from hardware SPI to software bit-banged SPI
  - LCD uses PB7-PB0 pins sequentially
  - PB5 (SCL) and PB3 (SDA) for SPI communication
  - PB7 (RST), PB6 (RS/DC), PB4 (CS)
  
- **Power Control**: Corrected power control logic
  - PC6: Main power control (inverted logic - LOW = ON)
  - PC5: 5V boost for audio amplifier  
  - PA3: LCD power control
  
- **ADC Inputs**: Fixed ADC channel assignments
  - PC2: Battery voltage (with 1:2 divider)
  - PC4: Left audio channel
  - PC3: Right audio channel
  
- **User Interface**: Updated button configuration
  - PB0: Power button (only button, mode switching via short press)
  - PC0/PC1: Red/Green LEDs (common anode, LOW = ON)

### Implementation Details
1. Converted hardware SPI to software bit-banged SPI for LCD communication
2. Implemented proper power control sequence considering transistor logic
3. Fixed battery voltage detection with correct divider ratio (2.0x)
4. Updated GPIO initialization with GPIOD clock enable workaround
5. Removed references to non-existent MODE button
6. Implemented two-level backlight control via Q7 transistor

### Technical Notes
- o3 verified the schematic uses ISP programming interface, not SWD
- PD6 is multiplexed between CHAR detection and UART RX
- Backlight control uses transistor switching instead of PWM

## Version History
- v3.0.1: Fixed Flash base address from 0x08000000 to 0x00000000
- v3.0.0: Initial release based on o3 analysis

## Files Included
- `firmware_v3.0.1_o3.hex` - Intel HEX format for programming (v3.0.1)
- `firmware_v3.0.1_o3.bin` - Binary format (v3.0.1)
- `firmware_v3.0_o3.hex` - Previous version (deprecated - wrong Flash address)
- `firmware_v3.0_o3.bin` - Previous version (deprecated - wrong Flash address)

## Programming Instructions
Use WB-Link with ISP interface (PC7 for clock/data) to program the firmware.
Base address: 0x00000000 (not 0x08000000 like STM32)