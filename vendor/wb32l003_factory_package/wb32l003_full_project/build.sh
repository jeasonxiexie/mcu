#!/bin/bash

# 编译脚本
# 用于编译WB32L003 VU表项目

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# 检查工具链
if ! command -v arm-none-eabi-gcc &> /dev/null
then
    echo -e "${RED}错误: 未找到arm-none-eabi-gcc${NC}"
    echo "请安装ARM嵌入式工具链"
    exit 1
fi

# 显示工具链版本
echo "使用工具链:"
arm-none-eabi-gcc --version | head -n 1

# 清理
if [ "$1" == "clean" ]; then
    echo "清理构建目录..."
    make clean
    exit 0
fi

# 构建项目
echo "开始编译..."
make -j4

# 检查结果
if [ $? -eq 0 ]; then
    echo -e "${GREEN}编译成功${NC}"
    echo "生成文件:"
    ls -la build/*.hex build/*.bin 2>/dev/null
    echo ""
    # 复制到firmware目录
    cp build/firmware.hex firmware/wb32l003_vu_meter.hex
    cp build/firmware.bin firmware/wb32l003_vu_meter.bin
    echo "固件已复制到firmware目录"
else
    echo -e "${RED}编译失败${NC}"
    exit 1
fi