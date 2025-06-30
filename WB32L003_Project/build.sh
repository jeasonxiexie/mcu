#!/bin/bash

# WB32L003 Project Build Script

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if arm-none-eabi-gcc is installed
if ! command -v arm-none-eabi-gcc &> /dev/null
then
    echo -e "${RED}Error: arm-none-eabi-gcc not found!${NC}"
    echo "Please install the ARM embedded toolchain:"
    echo "  macOS: brew install arm-none-eabi-gcc"
    echo "  Linux: sudo apt-get install gcc-arm-none-eabi"
    echo "  Windows: Download from https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm"
    exit 1
fi

# Display toolchain version
echo -e "${GREEN}Using toolchain:${NC}"
arm-none-eabi-gcc --version | head -n 1

# Clean build
if [ "$1" == "clean" ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    make clean
    exit 0
fi

# Build the project
echo -e "${YELLOW}Building WB32L003 Project...${NC}"
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1)

# Check if build was successful
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Build successful!${NC}"
    echo -e "${GREEN}Output files:${NC}"
    ls -la build/*.elf build/*.hex build/*.bin 2>/dev/null
    make size
else
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

# Flash command reminder
echo ""
echo -e "${YELLOW}To flash the firmware:${NC}"
echo "  ST-Link: st-flash write build/firmware.bin 0x08000000"
echo "  J-Link:  JLinkExe -device WB32L003K8U6 -if SWD -speed 4000 -CommanderScript flash.jlink"
echo ""
echo -e "${YELLOW}To convert images to C arrays:${NC}"
echo "  python3 tools/img2c.py UI_Assets/基地素材图.png background_base"