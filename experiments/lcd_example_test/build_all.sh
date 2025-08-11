#!/bin/bash

# LCD Example Test - 全版本编译脚本
# 用于编译所有LCD驱动版本并输出到统一目录
# 日期: 2024-08-06

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 打印带颜色的消息
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# 获取脚本所在目录
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# 定义输出目录
OUTPUT_DIR="${SCRIPT_DIR}/firmware_output"
VERSION1_OUTPUT="${OUTPUT_DIR}/version1_gpioc"
VERSION2_OUTPUT="${OUTPUT_DIR}/version2_gpiob"

# 版本目录
VERSION1_DIR="${SCRIPT_DIR}/v1_gpioc_standard_schematic"
VERSION2_DIR="${SCRIPT_DIR}/v2_gpiob_actual_hardware"

# 获取时间戳
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
BUILD_DATE=$(date +"%Y-%m-%d %H:%M:%S")

# 清理函数
cleanup() {
    print_info "清理旧的输出目录..."
    rm -rf "${OUTPUT_DIR}"
}

# 创建输出目录
create_directories() {
    print_info "创建输出目录结构..."
    mkdir -p "${VERSION1_OUTPUT}"
    mkdir -p "${VERSION2_OUTPUT}"
}

# 检查工具链
check_toolchain() {
    print_info "检查ARM工具链..."
    if ! command -v arm-none-eabi-gcc &> /dev/null; then
        print_error "未找到 arm-none-eabi-gcc"
        print_info "请安装ARM工具链:"
        print_info "  macOS: brew install arm-none-eabi-gcc"
        print_info "  Linux: sudo apt-get install gcc-arm-none-eabi"
        exit 1
    fi
    
    # 显示工具链版本
    GCC_VERSION=$(arm-none-eabi-gcc --version | head -n1)
    print_info "工具链版本: ${GCC_VERSION}"
}

# 编译Version 1
build_version1() {
    print_info "========================================="
    print_info "编译 Version 1 (GPIOC - 标准原理图版本)"
    print_info "========================================="
    
    cd "${VERSION1_DIR}"
    
    # 清理旧文件
    print_info "清理旧的编译文件..."
    make clean > /dev/null 2>&1 || true
    
    # 编译
    print_info "开始编译..."
    if make; then
        print_success "Version 1 编译成功"
        
        # 复制输出文件
        print_info "复制固件到输出目录..."
        cp lcd_test_v1.bin "${VERSION1_OUTPUT}/lcd_test_v1_${TIMESTAMP}.bin"
        cp lcd_test_v1.hex "${VERSION1_OUTPUT}/lcd_test_v1_${TIMESTAMP}.hex"
        cp lcd_test_v1.elf "${VERSION1_OUTPUT}/lcd_test_v1_${TIMESTAMP}.elf"
        
        # 创建符号链接到最新版本
        ln -sf "lcd_test_v1_${TIMESTAMP}.bin" "${VERSION1_OUTPUT}/lcd_test_v1_latest.bin"
        ln -sf "lcd_test_v1_${TIMESTAMP}.hex" "${VERSION1_OUTPUT}/lcd_test_v1_latest.hex"
        ln -sf "lcd_test_v1_${TIMESTAMP}.elf" "${VERSION1_OUTPUT}/lcd_test_v1_latest.elf"
        
        # 获取固件大小
        SIZE_INFO=$(arm-none-eabi-size lcd_test_v1.elf | tail -n1)
        
        # 保存编译信息
        cat > "${VERSION1_OUTPUT}/build_info.txt" << EOF
LCD Test Version 1 - GPIOC (标准原理图版本)
=====================================
编译时间: ${BUILD_DATE}
时间戳: ${TIMESTAMP}
GPIO端口: GPIOC
参考文档: 0.96寸插接式原理图.pdf

固件文件:
- lcd_test_v1_${TIMESTAMP}.bin
- lcd_test_v1_${TIMESTAMP}.hex
- lcd_test_v1_${TIMESTAMP}.elf

固件大小:
${SIZE_INFO}

引脚映射:
- LCD_RST: PC6
- LCD_RS:  PC5
- LCD_SCL: PC3
- LCD_CS:  PC1
- LCD_SDA: PC0
EOF
        
        print_success "Version 1 文件已保存到: ${VERSION1_OUTPUT}"
        return 0
    else
        print_error "Version 1 编译失败"
        return 1
    fi
}

# 编译Version 2
build_version2() {
    print_info "========================================="
    print_info "编译 Version 2 (GPIOB - 实际PCB版本)"
    print_info "========================================="
    
    cd "${VERSION2_DIR}"
    
    # 清理旧文件
    print_info "清理旧的编译文件..."
    make clean > /dev/null 2>&1 || true
    
    # 编译
    print_info "开始编译..."
    if make; then
        print_success "Version 2 编译成功"
        
        # 复制输出文件
        print_info "复制固件到输出目录..."
        cp lcd_test_v2.bin "${VERSION2_OUTPUT}/lcd_test_v2_${TIMESTAMP}.bin"
        cp lcd_test_v2.hex "${VERSION2_OUTPUT}/lcd_test_v2_${TIMESTAMP}.hex"
        cp lcd_test_v2.elf "${VERSION2_OUTPUT}/lcd_test_v2_${TIMESTAMP}.elf"
        
        # 创建符号链接到最新版本
        ln -sf "lcd_test_v2_${TIMESTAMP}.bin" "${VERSION2_OUTPUT}/lcd_test_v2_latest.bin"
        ln -sf "lcd_test_v2_${TIMESTAMP}.hex" "${VERSION2_OUTPUT}/lcd_test_v2_latest.hex"
        ln -sf "lcd_test_v2_${TIMESTAMP}.elf" "${VERSION2_OUTPUT}/lcd_test_v2_latest.elf"
        
        # 获取固件大小
        SIZE_INFO=$(arm-none-eabi-size lcd_test_v2.elf | tail -n1)
        
        # 保存编译信息
        cat > "${VERSION2_OUTPUT}/build_info.txt" << EOF
LCD Test Version 2 - GPIOB (实际PCB版本)
=====================================
编译时间: ${BUILD_DATE}
时间戳: ${TIMESTAMP}
GPIO端口: GPIOB
参考文档: WB32L003 耳放固件.rtf

固件文件:
- lcd_test_v2_${TIMESTAMP}.bin
- lcd_test_v2_${TIMESTAMP}.hex
- lcd_test_v2_${TIMESTAMP}.elf

固件大小:
${SIZE_INFO}

引脚映射:
- LCD_RST: PB7
- LCD_RS:  PB6
- LCD_SCL: PB5
- LCD_CS:  PB4
- LCD_SDA: PB3
- LCD_BL1: PB1 (背光控制)
- LCD_BL2: PB2 (背光亮度)
EOF
        
        print_success "Version 2 文件已保存到: ${VERSION2_OUTPUT}"
        return 0
    else
        print_error "Version 2 编译失败"
        return 1
    fi
}

# 生成总体报告
generate_report() {
    print_info "生成编译报告..."
    
    cat > "${OUTPUT_DIR}/BUILD_REPORT.md" << EOF
# LCD驱动固件编译报告

**编译时间**: ${BUILD_DATE}  
**时间戳**: ${TIMESTAMP}  
**工具链**: ${GCC_VERSION}

## 编译结果

### Version 1 - GPIOC (标准原理图版本)
- **状态**: ${VERSION1_STATUS}
- **输出目录**: \`version1_gpioc/\`
- **最新固件**: \`lcd_test_v1_latest.bin\`
- **参考文档**: 0.96寸插接式原理图.pdf

### Version 2 - GPIOB (实际PCB版本)
- **状态**: ${VERSION2_STATUS}
- **输出目录**: \`version2_gpiob/\`
- **最新固件**: \`lcd_test_v2_latest.bin\`
- **参考文档**: WB32L003 耳放固件.rtf

## 使用说明

### 烧录固件
1. 选择对应的版本（根据硬件）
2. 使用ISP烧录器连接到目标板
3. 烧录对应的.hex或.bin文件

### 版本选择指南
- **Version 1**: 用于标准0.96寸LCD模块开发板
- **Version 2**: 用于项目实际PCB板

## 注意事项
⚠️ 版本不对LCD不亮
⚠️ 请确认硬件连接后选择对应版本

---
*此报告由自动编译脚本生成*
EOF
    
    print_success "编译报告已生成: ${OUTPUT_DIR}/BUILD_REPORT.md"
}

# 主函数
main() {
    echo "========================================="
    echo "    LCD Example Test 全版本编译脚本"
    echo "========================================="
    echo
    
    # 检查工具链
    check_toolchain
    
    # 清理和创建目录
    cleanup
    create_directories
    
    # 编译两个版本
    VERSION1_STATUS="❌ 失败"
    VERSION2_STATUS="❌ 失败"
    
    if build_version1; then
        VERSION1_STATUS="✅ 成功"
    fi
    
    echo
    
    if build_version2; then
        VERSION2_STATUS="✅ 成功"
    fi
    
    echo
    
    # 生成报告
    generate_report
    
    # 显示最终结果
    echo
    echo "========================================="
    echo "              编译完成"
    echo "========================================="
    print_info "输出目录: ${OUTPUT_DIR}"
    print_info "Version 1 (GPIOC): ${VERSION1_STATUS}"
    print_info "Version 2 (GPIOB): ${VERSION2_STATUS}"
    echo
    
    # 返回状态
    if [[ "${VERSION1_STATUS}" == "✅ 成功" ]] && [[ "${VERSION2_STATUS}" == "✅ 成功" ]]; then
        print_success "所有版本编译成功！"
        exit 0
    else
        print_warning "部分版本编译失败，请检查错误信息"
        exit 1
    fi
}

# 执行主函数
main "$@"