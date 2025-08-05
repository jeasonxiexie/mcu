#!/bin/bash

# test_v1_backlight 编译脚本

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

# 检查工具链
if ! command -v arm-none-eabi-gcc &> /dev/null
then
    echo -e "${RED}错误: 未找到arm-none-eabi-gcc${NC}"
    exit 1
fi

# 创建Makefile
if [ ! -f Makefile ]; then
    cp ../Makefile.template Makefile
    sed -i '' 's/TARGET = test_v1/TARGET = test_v1_backlight/' Makefile
fi

# 清理或编译
if [ "$1" == "clean" ]; then
    make clean
else
    make clean
    make
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}编译成功${NC}"
        cp *.hex ../firmware/ 2>/dev/null
    else
        echo -e "${RED}编译失败${NC}"
        exit 1
    fi
fi