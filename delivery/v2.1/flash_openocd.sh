#!/bin/bash
# OpenOCD 烧录脚本

echo "WB32L003 OpenOCD 烧录脚本"
echo "========================"

# 检查参数
if [ $# -eq 0 ]; then
    echo "使用方法: ./flash_openocd.sh <hex文件>"
    echo "示例: ./flash_openocd.sh firmware_clean.hex"
    exit 1
fi

FIRMWARE=$1

# 检查文件是否存在
if [ ! -f "$FIRMWARE" ]; then
    echo "错误: 文件 $FIRMWARE 不存在"
    exit 1
fi

# 使用 OpenOCD 烧录
openocd -f wb32l003.cfg \
    -c "program $FIRMWARE verify reset exit" \
    || {
        echo "烧录失败！"
        echo "请检查："
        echo "1. 硬件连接是否正确"
        echo "2. OpenOCD 是否已安装"
        echo "3. 调试器驱动是否正常"
        exit 1
    }

echo "烧录成功！"