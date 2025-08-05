#!/bin/bash
# 编译所有LCD测试程序的脚本
# 需要先安装 arm-none-eabi-gcc 工具链

echo "开始编译所有LCD测试程序..."

# 检查工具链
if ! command -v arm-none-eabi-gcc &> /dev/null; then
    echo "错误: 未找到 arm-none-eabi-gcc"
    echo "请先安装 ARM 工具链"
    exit 1
fi

# 编译各个测试程序
for test_dir in test_v1_backlight test_v2_lcd_reset test_v3_full_display; do
    echo ""
    echo "编译 $test_dir..."
    
    # 进入目录
    cd $test_dir || exit 1
    
    # 复制Makefile模板
    cp ../Makefile.template Makefile
    
    # 修改TARGET名称
    case $test_dir in
        test_v1_backlight)
            sed -i '' 's/TARGET = test_v1/TARGET = test_v1_backlight_only/' Makefile
            ;;
        test_v2_lcd_reset)
            sed -i '' 's/TARGET = test_v1/TARGET = test_v2_backlight_reset/' Makefile
            ;;
        test_v3_full_display)
            sed -i '' 's/TARGET = test_v1/TARGET = test_v3_full_display/' Makefile
            ;;
    esac
    
    # 清理并编译
    make clean
    make
    
    # 检查结果
    if [ $? -eq 0 ]; then
        echo "✓ $test_dir 编译成功"
    else
        echo "✗ $test_dir 编译失败"
    fi
    
    # 返回上级目录
    cd ..
done

echo ""
echo "编译完成！"
echo "新的固件文件(.hex)已生成在各测试目录中"