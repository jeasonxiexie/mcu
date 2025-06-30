# WB32L003 Display Project

## Overview
This project implements a display system for the WB32L003K8U6 MCU with the following features:
- ST7735S TFT LCD driver (160x80, SPI interface)
- VU meter audio visualization
- Battery monitoring with 4-level indicator
- Low battery warning (LED flash at <3.0V)
- Auto shutdown at critical battery level (<2.8V)
- Low power standby mode (≤30μA)

## Hardware Configuration
- MCU: WB32L003K8U6 (Cortex-M0+, 64KB Flash, 8KB RAM)
- Display: 0.96" TFT ST7735S, 160×80 pixels, RGB565
- SPI: CLK=PA0, MOSI=PC15, CS=PC14, DC=PB3, RST=PB4
- Backlight: PB11 (TIM2_CH2 PWM)
- Battery ADC: PA6 (ADC_IN6)
- Power Button: PA7
- LED: PA2

## Project Structure
```
WB32L003_Project/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── st7735.h
│   │   ├── power.h
│   │   ├── ui.h
│   │   ├── wb32l003.h
│   │   └── wb32l003_hal_conf.h
│   └── Src/
│       ├── main.c
│       ├── st7735.c
│       ├── power.c
│       └── ui.c
├── Drivers/
│   ├── CMSIS/
│   └── WB32L003_HAL_Driver/
│       ├── Inc/
│       └── Src/
├── startup_wb32l003.s
├── WB32L003K8Ux_FLASH.ld
├── Makefile
├── build.sh
├── flash_stlink.sh
└── flash_jlink.sh
```

## Building the Project

### Prerequisites
Install the ARM embedded toolchain:
- macOS: `brew install arm-none-eabi-gcc`
- Linux: `sudo apt-get install gcc-arm-none-eabi`
- Windows: Download from ARM website

### Build Steps
```bash
# Make build script executable
chmod +x build.sh

# Build the project
./build.sh
```

This will generate:
- `build/WB32L003_Project.elf` - Debug file
- `build/WB32L003_Project.hex` - Intel HEX format
- `build/WB32L003_Project.bin` - Binary for flashing

## Flashing the Firmware

### Using ST-Link
```bash
chmod +x flash_stlink.sh
./flash_stlink.sh
```

### Using J-Link
```bash
chmod +x flash_jlink.sh
./flash_jlink.sh
```

## Features Implementation

### Display System
- Supports RGB565 format images
- Hardware SPI at 8MHz for fast updates
- PWM-controlled backlight (4 levels)
- VU meter visualization for audio

### Power Management
- Battery voltage monitoring via ADC
- 4-level battery indicator
- Low battery warning (LED flash)
- Auto shutdown at critical level
- STOP mode for low power (≤30μA)

### UI Resources
The project uses audio meter UI resources from the UI_Assets directory:
- Background image
- VU meter pointers
- Scale indicators

## Memory Usage
- Flash: ~28.7 KB / 64 KB (44.8%)
- RAM: ~2.3 KB / 8 KB (28.1%)

## Development Notes
- The HAL library has been simplified for the WB32L003K8U6
- All peripheral drivers are included
- The project is configured for optimization level -Og (debug)
- For production, change to -Os in Makefile

## License
This project is provided as-is for the WB32L003K8U6 development.