# WB32L003 VU Meter v2.1.0 Release Notes

## Release Date: 2025-07-10

## Major Updates

### Pin Configuration Updates (Based on Latest Schematic)
- Updated to match the latest hardware schematic from PCB designer
- Key pin changes:
  - SPI: PC5(SCK), PC6(MOSI), PC0(CS), PA3(DC), PD3(RST)
  - PWM: Currently using PA1 with TIM2 (will use PB1 with TIM1 when HAL supports it)
  - Buttons: PD6(PWR), PD4(MODE)
  - LEDs: PB0(RED), PB2(GREEN)
  - ADC: PC0(BAT/AIN15), PC1(L_AD/AIN14), PC2(R_AD/AIN13)

### Code Improvements
- Added GPIOD support (partially - full support pending HAL update)
- Implemented PD6 pin multiplexing for KEY_PWR/UART_RX
- Added ADC calibration code
- Optimized SPI for single-direction mode (TX only)
- Updated display reset timing (≥15ms)
- Added clock source selection in Makefile (USE_HSE=1 for external crystal)

### Known Limitations
- GPIOD pins (PD3, PD4, PD6) are defined but not initialized due to HAL limitations
- TIM1 not available in current HAL, using TIM2 for PWM temporarily
- PC0 is shared between TFT_CS and BAT_ADC (requires hardware isolation)

## Firmware Details
- Size: ~64KB (near Flash limit)
- Target: WB32L003K8U6 (QFN32)
- Clock: 24MHz HSI (internal)

## Flashing Instructions
1. Connect ST-Link V2 to SWD interface:
   - VCC → 3.3V
   - SWDIO → PC7
   - SWCLK → PD1
   - GND → GND

2. Flash using your preferred tool:
   - STM32CubeProgrammer (Windows)
   - st-flash (macOS/Linux): `st-flash write firmware.bin 0x08000000`

## Next Steps
- Wait for updated HAL library with full GPIOD and TIM1 support
- Test on actual hardware when PCB arrives
- Fine-tune ADC calibration and power management

## Notes
This release is prepared for the latest schematic but has some features disabled due to HAL limitations. The firmware is ready for testing once the hardware is available.