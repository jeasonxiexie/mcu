# STM32 Clean Architecture Template

A clean architecture framework for rapid STM32 project development with configurable hardware mappings and modular business logic.

## 🏗️ Architecture Overview

This template follows **Clean Architecture** principles, separating business logic from hardware implementation:

```
┌─ app/                    # Application layer (main.c, pin_config)
├─ core/                   # Business logic (domain independent)
│  ├─ domain/              # Core types and enums
│  ├─ interfaces/          # Hardware abstraction interfaces
│  └─ usecases/           # Business logic implementation
├─ adapters/              # Hardware interface implementations
│  ├─ display/            # Display driver adapters
│  ├─ power/              # Power management adapters
│  ├─ input/              # Input device adapters
│  └─ audio/              # Audio system adapters
├─ frameworks/            # Hardware-specific drivers
│  ├─ hal/                # HAL library integration
│  ├─ drivers/            # Peripheral drivers (ST7735, etc.)
│  ├─ cmsis/              # CMSIS library
│  └─ startup/            # MCU startup files
└─ config/                # JSON configuration files
```

## 🚀 Quick Start

### 1. Generate a New Project

```bash
cd stm32-clean-template/tools
python3 project_generator.py MyProject --mcu STM32F030K6 --display st7735
```

### 2. Configure Hardware

Edit `config/pinmap.json` to match your hardware:

```json
{
  "mcu": "STM32F030K6",
  "peripherals": {
    "display": {
      "type": "st7735",
      "interface": "spi1",
      "cs": "PB0",
      "dc": "PB1", 
      "rst": "PB2",
      "bl": "PA1"
    },
    "power": {
      "led": "PA4",
      "control": "PA5"
    }
  }
}
```

### 3. Add Image Resources

**Simple Way:**
```bash
# Put your images in assets/images/
cp boot_logo.png assets/images/
cp meter_animation.gif assets/images/

# Convert all images to C arrays
python tools/convert_assets.py --generate-integration
```

**Manual Way:**
```bash
# Convert individual images
python tools/img2c.py assets/images/boot_logo.png
python tools/img2c.py assets/images/meter_animation.gif
```

### 4. Configure Features

- Edit `config/features.json` for application settings
- Customize business logic in `core/usecases/`
- Use generated image resources in your UI code

### 5. Build

```bash
make clean
make
```

## 🎯 Key Features

### ✅ Hardware Abstraction
- **Interface-based design** - swap hardware implementations easily
- **JSON configuration** - no code changes for pin mapping
- **Multi-MCU support** - works with any STM32 family

### ✅ Ready-to-Use Components
- **ST7735 Display Driver** - optimized for 80x160 displays
- **Power Management** - battery monitoring, low power modes
- **Audio Management** - VU meter, stereo/mono switching
- **Input Handling** - button debouncing, long press detection

### ✅ Business Logic
- **Power Management Use Case** - handles battery warnings, shutdown
- **Audio Management Use Case** - processes audio levels and modes
- **UI Management Use Case** - orchestrates display updates

### ✅ Development Tools
- **Project Generator** - creates new projects from templates
- **Image Converter** - PNG/GIF to C arrays with one command
- **Batch Asset Manager** - converts and integrates all resources
- **Makefile Templates** - automated build configuration
- **Pin Configuration Generator** - generates pin_config.h from JSON

## 📋 Supported Hardware

### MCU Families
- STM32F0xx (Cortex-M0)
- STM32F1xx (Cortex-M3)
- STM32F2xx (Cortex-M3)
- STM32F3xx (Cortex-M4)
- STM32F4xx (Cortex-M4)
- STM32F7xx (Cortex-M7)
- STM32L0xx (Cortex-M0+)
- STM32L1xx (Cortex-M3)
- STM32L4xx (Cortex-M4)
- STM32G0xx (Cortex-M0+)
- STM32G4xx (Cortex-M4)
- STM32H7xx (Cortex-M7)

### Displays
- ST7735 (80x160, SPI interface)
- Extensible for other display controllers

### Peripherals
- SPI (1MHz-20MHz, configurable)
- ADC (12-bit, battery monitoring)
- PWM (backlight control)
- GPIO (buttons, LEDs, control signals)

## 🔧 Configuration Files

### `config/pinmap.json`
Hardware pin assignments and peripheral configuration:

```json
{
  "mcu": "STM32F030K6",
  "peripherals": {
    "spi1": {
      "sck": "PA5",
      "miso": "PA6", 
      "mosi": "PA7"
    },
    "adc": {
      "battery": {
        "channel": "ADC_CHANNEL_0",
        "pin": "PA0"
      }
    }
  }
}
```

### `config/features.json`
Application features and settings:

```json
{
  "display": {
    "width": 80,
    "height": 160,
    "fps": 20,
    "backlight_pwm": true
  },
  "power": {
    "low_voltage_threshold": 3.0,
    "critical_voltage_threshold": 2.8,
    "auto_shutdown": true
  }
}
```

### `config/board_config.json`
MCU and toolchain configuration:

```json
{
  "project": {
    "name": "MyProject",
    "version": "1.0.0"
  },
  "mcu": {
    "model": "STM32F030K6",
    "series": "stm32f0",
    "flash_size": "32K",
    "ram_size": "4K"
  }
}
```

## 🎨 Adding Custom Business Logic

### 1. Create Use Case

```c
// core/usecases/my_feature.h
typedef struct {
    bool (*init)(const my_interface_t* interface);
    void (*process)(void);
    my_status_t (*get_status)(void);
} my_feature_usecase_t;
```

### 2. Implement Interface

```c
// core/interfaces/my_interface.h
typedef struct {
    bool (*init)(void);
    void (*do_something)(uint32_t value);
    uint32_t (*get_data)(void);
} my_interface_t;
```

### 3. Create Adapter

```c
// adapters/my_device/my_adapter.c
static bool my_init(void) {
    // Hardware-specific initialization
    return true;
}

const my_interface_t my_adapter_interface = {
    .init = my_init,
    .do_something = my_do_something,
    .get_data = my_get_data
};
```

### 4. Wire in Main

```c
// app/main.c
#include "adapters/my_device/my_adapter.h"

static bool Initialize_Interfaces(void) {
    if (!my_feature_usecase.init(&my_adapter_interface)) {
        return false;
    }
    return true;
}
```

## 📊 Example Projects

### VU Meter with ST7735
Complete audio level meter with stereo/mono switching:

```bash
python3 project_generator.py VUMeter \
  --mcu STM32F030K6 \
  --display st7735 \
  --config features.audio.enabled=true \
  --config features.power.battery_monitoring=true
```

Features:
- Real-time audio level display
- Battery voltage monitoring  
- Power button (short press = mode toggle, long press = shutdown)
- Low power sleep mode
- Optimized for 64KB flash

### Basic Display Project
Simple display control project:

```bash
python3 project_generator.py DisplayTest \
  --mcu STM32L0xx \
  --display st7735
```

## 🔨 Build System

### Makefile Generation
The build system automatically generates optimized Makefiles based on your configuration:

- **MCU-specific** compiler flags and linker scripts
- **Automatic** source file discovery
- **Debug/Release** configurations
- **Size optimization** for resource-constrained MCUs

### Build Commands
```bash
# Clean build
make clean

# Debug build
make DEBUG=1

# Release build (optimized)
make

# Show memory usage
make size

# Generate binary and hex files
make all
```

## 🚨 Error Handling

The template includes comprehensive error handling:

### Power Management
- **Low battery warning** (< 3.0V) - flashes power LED
- **Critical battery** (< 2.8V) - automatic shutdown
- **Power button timeout** - graceful shutdown after 10s

### Display Management  
- **Init failure recovery** - retries with fallback settings
- **Memory constraints** - automatic optimization for limited flash
- **SPI communication errors** - timeout and retry logic

### Input Validation
- **Button debouncing** - configurable debounce timing
- **Long press detection** - prevents accidental shutdowns
- **Interface validation** - null pointer protection

## 📈 Performance

### Memory Usage
- **Flash**: 15-25KB (depending on features)
- **RAM**: 2-4KB (typical usage)
- **Optimized** for 32KB flash / 4KB RAM MCUs

### Real-time Performance
- **20 FPS** display updates
- **50ms** audio level polling
- **1ms** main loop cycle
- **<30µA** sleep mode current

## 🤝 Contributing

1. **Fork** the repository
2. **Create** a feature branch
3. **Follow** clean architecture principles
4. **Add** tests for new adapters
5. **Submit** a pull request

### Code Style
- Use **interface-based** design for hardware abstraction
- Keep **business logic** in use cases
- Make **adapters** hardware-specific only
- Write **self-documenting** code with clear naming

## 📄 License

MIT License - see LICENSE file for details.

## 🎯 Roadmap

- [ ] **CAN bus** interface and adapter
- [ ] **I2C sensor** framework  
- [ ] **UART communication** adapter
- [ ] **File system** integration
- [ ] **Real-time OS** support
- [ ] **Unit testing** framework
- [ ] **Hardware-in-the-loop** testing
- [ ] **Code generation** from CubeMX projects

---

*Built with ❤️ for the STM32 community*