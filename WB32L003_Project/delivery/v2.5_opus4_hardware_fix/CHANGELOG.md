# Firmware Version 2.5 - Opus4 Hardware Verification Fix

## Release Date: 2025-07-31

## Overview
This firmware version implements hardware fixes based on Opus4's schematic analysis and verification.

## Major Changes

### Pin Configuration Updates (based on Opus4 verification)
- **SPI Interface**: Using hardware SPI on PC5/PC6
  - PC5: SPI Clock (LCD_SCL)
  - PC6: SPI Data (LCD_SDA)
  
- **LCD Control Pins**: 
  - PB7: LCD_CS (Chip Select)
  - PA4: LCD_RS (Data/Command)
  - PA2: LCD_RESET (using OSCOUT pin)
  
- **Backlight Control**: Two-level brightness control
  - PD0: CLD_BL1 (High brightness)
  - PB6: CLD_BL2 (Low brightness)
  
- **ADC Inputs**: Using PB0/PB1/PB2
  - PB2: Battery voltage (with 1:4.3 divider)
  - PB0: Left audio channel
  - PB1: Right audio channel
  
- **User Interface**: 
  - PD6: Power button
  - PD5: Mode/Stereo button
  - PD3: Red LED
  - PD2: Green LED

### Power Control Updates
- PC4: Main power control (CON_POW_CPU)
- PB6: LCD power control (shared with BL2)
- PC1: RF/5V power control

### Audio Control
- PC3: Mute control
- PC2: Stereo/Mono selection

### Implementation Details
1. Hardware SPI for LCD communication (faster than bit-banged)
2. Two-level backlight control using GPIO (not PWM)
3. Fixed battery voltage detection with 4.3x divider ratio
4. Updated GPIO initialization with GPIOD clock enable workaround
5. Dual button support (power and mode)

### Technical Notes
- Uses standard SPI interface for LCD
- Backlight control via transistor switching
- Supports both power and mode buttons
- LED control with active-low logic

## Files Included
- `firmware_v2.5_opus4.hex` - Intel HEX format for programming
- `firmware_v2.5_opus4.bin` - Binary format

## Programming Instructions
Use WB-Link with ISP interface (PC7 for clock/data) to program the firmware.
Base address: 0x00000000 (not 0x08000000 like STM32)