# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a WB32L003 MCU-based VU meter project with a 0.96" TFT display (ST7735S controller, 80x160 pixels). The project uses software SPI for LCD communication and includes audio visualization features.

## Critical Hardware Configuration

**IMPORTANT**: All LCD pins use GPIOC (not GPIOB - this was a critical bug that was fixed):
- LCD_SCK: PC5 (SPI Clock)
- LCD_MOSI: PC6 (SPI Data) 
- LCD_CS: PC0 (Chip Select)
- LCD_RS: PC1 (Data/Command)
- LCD_RST: PC3 (Reset)
- LCD_BL1: PB1 (Backlight control)
- Power Control: PC6 (Low=ON, High=OFF)

## Build Commands

```bash
# Build default firmware (no LCD offset)
./build.sh
# or
make

# Build with specific LCD variant (for different display panels)
make VARIANT=lcd_v2_offset_24  # Most common for BOE panels
make VARIANT=lcd_v1_no_offset   # No X-axis offset
make VARIANT=lcd_v3_offset_26   # 26 pixel offset
make VARIANT=lcd_v4_offset_32   # 32 pixel offset

# Clean build artifacts
make clean

# Display binary size
make size
```

## Testing and Validation

There are no automated tests or linting commands currently set up. Validation is done through:
- Hardware testing with actual display panels
- Manual verification of display output
- Size constraints check (firmware must fit in 32KB flash)

## Project Architecture

```
WB32L003_Project/
├── Core/
│   ├── Inc/          # Header files (pin_config.h, st7735.h, etc.)
│   └── Src/          # Implementation files
│       ├── main.c    # Main application entry
│       ├── st7735.c  # LCD driver
│       ├── ui.c      # User interface rendering
│       └── hardware_control.c  # Hardware abstraction
├── Drivers/          # WB32 HAL drivers
├── build/           # Build output (firmware.hex, firmware.bin)
└── delivery/        # Release versions history
```

## Key Development Considerations

1. **Memory Constraints**: Only 32KB flash and 4KB RAM available. Code must be highly optimized.

2. **LCD Variants**: Different display panels require different X-axis offsets. The VARIANT build option selects the appropriate LCD driver.

3. **Power Management**: PC6 controls main power (active low). Early power initialization is critical for stable operation.

4. **Display Protocol**: Software SPI implementation with specific timing requirements for ST7735S controller.

5. **Flash Address**: Flash starts at 0x08000000 (not 0x00000000) for this MCU.

## Common Tasks

### Adding a new LCD variant
1. Copy an existing lcd_vX_*.c file in Core/Src/
2. Modify the X-axis offset in LCD_SetAddress() function
3. Update Makefile to include the new variant option

### Debugging display issues
- Check pin connections (ensure GPIOC is used for LCD)
- Try different VARIANT options if display is offset
- Verify power sequencing (PC6 must be low before LCD init)

### Flashing firmware
```bash
# Using J-Link
JLinkExe -device WB32L003K8U6 -if SWD -speed 4000 -CommandFile flash.jlink

# Using ST-Link (if compatible)
st-flash write build/firmware.bin 0x08000000
```

## Important Notes

- This is an embedded project with no OS - all code runs bare metal
- The project was originally misconfigured with GPIOB for LCD pins but has been corrected to GPIOC
- The delivery/ directory contains historical releases showing the evolution of fixes
- Test files have been moved to experiments/test_sources/ to keep the main build clean