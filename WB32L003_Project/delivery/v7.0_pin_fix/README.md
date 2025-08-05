# v7.0 Pin Fix - Critical Hardware Mapping Correction

## Problem Identified

O3 discovered that the LCD pins in the code were mapped to the wrong GPIO port:

**Previous (Wrong) Mapping:**
- LCD pins were on GPIOB (PB7-PB3)
- This explains why nothing worked - we were sending SPI signals to empty pins!

**Correct Mapping (from schematic):**
| LCD Signal | Correct Pin | Was Using |
|------------|-------------|-----------|
| SCK        | PC5         | PB5       |
| MOSI       | PC6         | PB3       |
| CS         | PC0         | PB4       |
| RS/DC      | PC1         | PB6       |
| RESET      | PC3         | PB7       |
| BLK(H)     | PB1         | PB1 ✓     |
| BLK(L)     | PB2         | PB2 ✓     |

## Changes Made

1. **pin_config.h**: Updated all LCD pin definitions from GPIOB to GPIOC
2. **LCD initialization**: Added GPIOC clock enable
3. **Backlight pins**: Kept on GPIOB (already correct)

## Test Files

- `lcd_v1_no_offset.hex` - Basic test with corrected pin mappings

## Expected Result

With the correct pin mappings, the LCD should finally respond to commands and display the red test pattern.

## Next Steps

If this works, we can test the different offset versions (24, 26, 32) to find the correct display alignment.