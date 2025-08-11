#!/bin/bash

# Version 1 (GPIOC) 独立编译脚本
# 基于标准原理图的LCD驱动版本

set -e

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

echo "====================================="
echo "  Version 1 (GPIOC) 固件编译"
echo "====================================="
echo

# 检查工具链
if ! command -v arm-none-eabi-gcc &> /dev/null; then
    echo -e "${RED}[ERROR]${NC} 未找到ARM工具链"
    echo "请安装: brew install arm-none-eabi-gcc (macOS)"
    exit 1
fi

# 清理
echo "清理旧文件..."
make clean > /dev/null 2>&1 || true

# 编译
echo "开始编译..."
if make; then
    echo -e "${GREEN}[SUCCESS]${NC} 编译成功!"
    echo
    echo "生成的固件文件:"
    echo "  - lcd_test_v1.bin (烧录文件)"
    echo "  - lcd_test_v1.hex (烧录文件,可选)"
    echo "  - lcd_test_v1.elf (调试文件)"
    echo
    arm-none-eabi-size lcd_test_v1.elf
else
    echo -e "${RED}[ERROR]${NC} 编译失败"
    exit 1
fi