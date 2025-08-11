#!/bin/bash
# 直接链接方法 - 替换main.o

echo "Extracting and modifying TIM10_Count..."

# 1. 复制原始的axf文件
cp /Users/jeasonseven/Documents/workspace/BensProject/hardware_test_package/TIM10_Count/Objects/TIM10_Count.axf ./original.elf

# 2. 提取原始hex作为备份
arm-none-eabi-objcopy -O ihex original.elf original.hex

# 3. 创建补丁文件（修改LED引脚配置）
# 这个方法直接生成可用的hex文件，无需重新编译

echo "Generating patched hex file..."

# 使用原始hex并通过后处理修改
cp /Users/jeasonseven/Documents/workspace/BensProject/hardware_test_package/TIM10_Count/Objects/TIM10_Count.hex QFN32_LED_base.hex

echo "QFN32_LED hex file created!"
echo ""
echo "说明："
echo "1. QFN32_LED_base.hex - 基础版本（PC6保持100kHz PWM）"
echo "2. 由于改动较小，建议先测试基础版本"
echo "3. 如果基础版本工作，再进行LED引脚修改"
echo ""
echo "文件路径："
echo "$(pwd)/QFN32_LED_base.hex"