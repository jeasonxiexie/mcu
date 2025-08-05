#!/bin/bash

# LCD测试程序编译脚本
# 编译所有LCD测试程序

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
echo ""

# 测试程序列表
TESTS=("test_v1_backlight" "test_v2_lcd_reset" "test_v3_full_display")

# 清理模式
if [ "$1" == "clean" ]; then
    echo "清理所有构建文件..."
    for test in "${TESTS[@]}"; do
        echo "清理 $test..."
        cd $test
        if [ -f Makefile ]; then
            make clean
        fi
        rm -f *.o *.elf *.map *.lst *.d
        cd ..
    done
    echo -e "${GREEN}清理完成${NC}"
    exit 0
fi

# 编译所有测试程序
echo "开始编译LCD测试程序..."
echo "========================"

SUCCESS=0
FAILED=0

for test in "${TESTS[@]}"; do
    echo ""
    echo -e "${YELLOW}编译 $test...${NC}"
    echo "------------------------"
    
    cd $test
    
    # 创建Makefile（如果不存在）
    if [ ! -f Makefile ]; then
        cp ../Makefile.template Makefile
        # 根据目录名设置TARGET
        case $test in
            test_v1_backlight)
                sed -i '' 's/TARGET = test_v1/TARGET = test_v1_backlight/' Makefile
                ;;
            test_v2_lcd_reset)
                sed -i '' 's/TARGET = test_v1/TARGET = test_v2_lcd_reset/' Makefile
                ;;
            test_v3_full_display)
                sed -i '' 's/TARGET = test_v1/TARGET = test_v3_full_display/' Makefile
                ;;
        esac
    fi
    
    # 编译
    make clean > /dev/null 2>&1
    make
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ $test 编译成功${NC}"
        # 复制到firmware目录
        cp *.hex ../firmware/ 2>/dev/null
        ((SUCCESS++))
    else
        echo -e "${RED}✗ $test 编译失败${NC}"
        ((FAILED++))
    fi
    
    cd ..
done

# 总结
echo ""
echo "========================"
echo "编译完成:"
echo -e "成功: ${GREEN}$SUCCESS${NC}"
echo -e "失败: ${RED}$FAILED${NC}"

if [ $SUCCESS -gt 0 ]; then
    echo ""
    echo "固件文件已保存到 firmware/ 目录:"
    ls -la firmware/*.hex
fi