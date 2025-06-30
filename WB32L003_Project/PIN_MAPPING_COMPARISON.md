# Pin Mapping Comparison Report

## Summary
Based on the comparison between `project_spec.md` and the new `specs.md` (extracted from TFT096B039.pdf and 耳放项目方案.docx), the current pin configuration correctly implements the display interface but lacks several important control pins.

## Pin Status

### ✅ Correctly Implemented
1. **Display SPI Interface**
   - TFT_SCLK: PA5 (SPI1_SCK) ✓
   - TFT_MOSI: PA7 (SPI1_MOSI) ✓
   - TFT_CS: PB0 (Chip Select) ✓
   - TFT_DC: PB1 (Data/Command, called RS in specs) ✓
   - TFT_RST: PB2 (Hardware Reset) ✓

2. **Backlight Control**
   - BL_EN/TFT_BL: PA1 (PWM control) ✓
   - Supports 5mA/12mA/20mA levels ✓

3. **Basic Power Management**
   - BAT_ADC: PA0 (Battery voltage monitoring) ✓
   - KEY_PWR: PC13 (Power button) ✓

### ❌ Missing Pins (from specs.md)
1. **Power Control**
   - CON_POW: Main power control pin
   - CON_LCD: LCD power control pin
   
2. **Audio Control**
   - MUTE: Audio mute control (High = unmute)
   
3. **Battery Management**
   - CHRG: Charging detection (Low = charging)
   
4. **Audio Level Detection**
   - L_AD: Left channel audio ADC input
   - R_AD: Right channel audio ADC input

## Required Actions

### 1. Hardware Design Review
You need to assign the missing pins based on available GPIO on WB32L003K8U6:
- Check which pins are still available
- Consider ADC capability for L_AD and R_AD
- Ensure proper GPIO for digital control signals

### 2. Pin Assignment Suggestions
Based on typical WB32L003 pinout, consider:
- CON_POW: PA3 or PA4 (general GPIO)
- CON_LCD: PA6 (if not used for SPI)
- MUTE: PB3 or PB4 (general GPIO)
- CHRG: PA8 or PB5 (input with pull-up)
- L_AD: PA4 (ADC_CHANNEL_4)
- R_AD: PA5 (ADC_CHANNEL_5) - Note: Conflicts with SPI SCK

### 3. Software Updates Required
Once pins are assigned:
1. Uncomment and update pin definitions in `pin_config.h`
2. Update `power.c` to use CON_POW and CON_LCD
3. Implement proper power sequencing:
   - Power on: CON_POW=1, CON_LCD=1 → Boot screen → MUTE=1
   - Power off: MUTE=0 → Off screen → CON_POW=1, CON_LCD=1 → Sleep
4. Add charging detection in battery monitoring
5. Implement audio level detection for VU meter

## Display Specifications Confirmed
- Resolution: 80×160 RGB (portrait orientation)
- Driver: ST7735S
- Interface: 4-wire SPI
- Voltage: 2.5-3.3V logic, 2.9-3.1V backlight
- Current: ~2mA logic, 15-20mA backlight

## Next Steps
1. Confirm available GPIO pins on your hardware
2. Assign the missing pins
3. Update pin_config.h with actual pin assignments
4. Implement the enhanced power management sequence
5. Add audio level monitoring functionality