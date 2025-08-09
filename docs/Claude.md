# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

WB32L003 MCU-based VU meter project with 0.96" TFT display (ST7735S controller). This is an embedded firmware project focused on hardware-correct implementation.

## Build Commands

```bash
# From WB32L003_Project directory
./build.sh                        # Standard build with default variant

# Build with specific LCD variant (based on actual Makefile)
make VARIANT=lcd_v1_no_offset    # Default - no offset
make VARIANT=lcd_v2_offset_24    # 24 pixel offset variant  
make VARIANT=lcd_v3_offset_26    # 26 pixel offset variant
make VARIANT=lcd_v4_offset_32    # 32 pixel offset variant

# Other make targets
make clean                        # Clean build artifacts
make size                         # Display firmware size info
make flash                        # Flash instructions (requires J-Link/WB-Link)
```

## Testing Approach

The project uses hardware validation testing rather than software unit tests:

1. **Test Sources**: `/experiments/test_sources/` contains 20+ hardware test programs:
   - `minimal_test.c`, `power_test.c`, `screen_test.c` etc.
   - Each tests specific hardware functionality

2. **Test Builds**: `/experiments/makefiles/` contains specialized Makefiles for test builds:
   - `Makefile_minimal`, `Makefile_power_test`, `Makefile_screen_test` etc.

3. **Diagnostic Firmware**: Production test versions in `/WB32L003_Project/delivery/v*.0_*/`

## Core Architecture

### Critical Pin Configuration
From `/WB32L003_Project/Core/Inc/pin_config.h` (Hardware team verified - v3.3):

```c
// LCD Interface - GPIOC pins
#define LCD_RST_PIN   GPIO_PIN_3  // PC3 - Reset
#define LCD_RS_PIN    GPIO_PIN_1  // PC1 - Data/Command  
#define LCD_SCL_PIN   GPIO_PIN_5  // PC5 - SPI Clock
#define LCD_CS_PIN    GPIO_PIN_0  // PC0 - Chip Select
#define LCD_SDA_PIN   GPIO_PIN_6  // PC6 - SPI Data

// Backlight control - GPIOB
#define CLD_BL1_PIN   GPIO_PIN_1  // PB1 - High brightness
#define CLD_BL2_PIN   GPIO_PIN_2  // PB2 - Low brightness

// Power control
#define LCD_POWER_PIN GPIO_PIN_3  // PA3 - LCD Power (Active Low)
```

### Key Source Files

**Core Functionality:**
- `/Core/Src/main.c` - Main application
- `/Core/Src/early_power.c` - Critical power latch initialization
- `/Core/Src/st7735.c` - LCD driver implementation
- `/Core/Src/hardware_control.c` - Hardware abstraction layer
- `/Core/Src/power.c` - Power management
- `/Core/Src/audio_adc.c` - Audio level detection

**Build Configuration:**
- `/WB32L003_Project/Makefile` - Main build configuration
- `/WB32L003_Project/WB32L003K8Ux_FLASH.ld` - Linker script
- `/WB32L003_Project/startup_wb32l003.s` - Startup assembly

## Important Implementation Notes

1. **Early Power Control**: The `early_power.c` must execute before any GPIO initialization to maintain power latch

2. **LCD Variants**: Different LCD panels require different X/Y offsets, controlled via VARIANT build option

3. **Software SPI**: Display uses software-implemented SPI on GPIOC pins

4. **Power Pin**: PA3 controls LCD power supply (active low) - critical for display operation

5. **Memory Constraints**: Firmware must fit in 64KB flash (WB32L003K8U6 limitation)

## Development Workflow

1. Test new features in `/experiments/test_sources/` first
2. Use specialized test Makefiles in `/experiments/makefiles/`
3. Integrate tested code into main project
4. Create versioned delivery in `/WB32L003_Project/delivery/`

## Module Structure

The project uses a hierarchical CLAUDE.md documentation system:
- Root `CLAUDE.md` - Overall project structure
- `/WB32L003_Project/CLAUDE.md` - Main firmware details
- `/experiments/CLAUDE.md` - Test development notes
- `/docs/CLAUDE.md` - This file with technical guidance

Each module has specific task tracking in its CLAUDE.md file.