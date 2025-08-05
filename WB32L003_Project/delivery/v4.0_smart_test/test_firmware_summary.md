# WB32L003 LCD Test Firmware Summary

## Overview
Generated 20 smart test firmware variants to systematically identify the correct configuration for the 0.96" TFT LCD display.

## Test Strategy
Instead of testing all 864 possible combinations, we created 20 key variants that cover the most likely configurations based on the analysis of demo code and documentation.

## Phase 1: Power Verification (3 variants)
These test basic power control and ensure the device can start up correctly.

1. **v1_simple_blink.hex** - Simple blink test with early power latch
2. **v2_early_power.hex** - Early power latch with button checking  
3. **v3_screen_basic.hex** - Basic screen initialization with power management

## Phase 2: Screen Configuration Tests (10 variants)
These test the most common LCD configurations found in the demo code.

4. **v4_x24_vcom06_portrait.hex** - X offset=24, VCOM=0x06, Portrait (BOE standard)
5. **v5_x24_vcom09_portrait.hex** - X offset=24, VCOM=0x09, Portrait
6. **v6_x0_vcom06_portrait.hex** - No X offset, VCOM=0x06, Portrait
7. **v7_x0_vcom09_portrait.hex** - No X offset, VCOM=0x09, Portrait
8. **v8_x26_vcom06_portrait.hex** - X offset=26, VCOM=0x06, Portrait
9. **v9_x32_vcom06_portrait.hex** - X offset=32, VCOM=0x06, Portrait
10. **v10_x24_vcom06_landscape.hex** - X offset=24, VCOM=0x06, Landscape
11. **v11_x24_vcom09_landscape.hex** - X offset=24, VCOM=0x09, Landscape
12. **v12_x24_vcom06_inverted.hex** - Inverted color test
13. **v13_boe_exact.hex** - Exact BOE demo configuration

## Phase 3: Extended Tests (7 variants)
These test edge cases and alternative configurations.

14. **v14_y_offset_landscape.hex** - Y-axis offset=24 for landscape mode
15. **v15_x24_vcom1d_portrait.hex** - VCOM=0x1D test
16. **v16_backlight_high.hex** - Alternative backlight control (HIGH to turn on)
17. **v17_minimal_init.hex** - Minimal initialization sequence
18. **v18_rotate180.hex** - 180-degree rotation test
19. **v19_inversion.hex** - Display inversion enabled
20. **v20_bgr_swap.hex** - BGR color order test

## Key Parameters Tested

### X-axis Offset
- 0 pixels (no offset)
- 24 pixels (BOE standard)
- 26 pixels
- 32 pixels

### VCOM Values
- 0x06 (most common)
- 0x09 (alternative)
- 0x1D (rare)

### Display Orientations
- Portrait (0x08)
- Landscape (0xC8)
- 180° rotation (0xC0)
- Inverted (0x48)
- BGR swap (0x00)

### Special Tests
- Y-axis offset for landscape
- Display inversion
- Alternative backlight control
- Minimal initialization

## Testing Instructions

1. Start with Phase 1 to verify basic power control
2. If power is OK, test Phase 2 variants (v4 is most likely to work)
3. If none work, try Phase 3 extended tests
4. Note which variant successfully displays content

## Pin Configuration
All variants use the same pin mapping:
- CS: PA4
- DC/RS: PB0  
- SCK: PA5
- SDA: PA7
- RST: PB1
- BL: PC7 (LOW = ON for S8550 PNP transistor)
- Power Latch: PC6 (LOW = ON)

## Expected Display Patterns
- Phase 1: Simple patterns or solid colors
- Phase 2: Color stripes or gradients
- Phase 3: Various test patterns (checkerboard, gradient, solid red)

## Success Indicators
- Device stays powered on after USB disconnect
- Backlight turns on
- Display shows the expected pattern
- Colors appear correct (not inverted or swapped)