#!/usr/bin/env python3
"""
直接修改hex文件，把PC4改为PC0
在GPIO操作中，Pin号的区别：
- GPIO_Pin_4 = 0x0010 (位4)
- GPIO_Pin_0 = 0x0001 (位0)
"""

# 读取原始hex和二进制
with open('original_working.hex', 'r') as f:
    hex_lines = f.readlines()

# Intel HEX 转二进制
import subprocess
subprocess.run(['arm-none-eabi-objcopy', '-I', 'ihex', '-O', 'binary', 
                'original_working.hex', 'original.bin'])

# 读取二进制
with open('original.bin', 'rb') as f:
    binary = bytearray(f.read())

# 搜索并替换
# 在ARM Thumb代码中，立即数0x10通常编码为：
# MOVS Rx, #0x10 -> 20 10 (little endian)
# 需要改为
# MOVS Rx, #0x01 -> 20 01

modified = False

# 查找TIM10_IRQHandler中的GPIO操作
# GPIO_ToggleBits通常会用到pin mask
# 在中断处理中查找0x10的使用

print("分析二进制...")
for i in range(len(binary) - 1):
    # 查找可能的PC4引用
    if binary[i] == 0x10 and binary[i+1] == 0x00:
        # 检查上下文，看是否是GPIO操作
        if i > 10 and i < len(binary) - 10:
            # 打印周围的字节以便分析
            context = binary[i-10:i+10].hex()
            print(f"位置 0x{i:04x}: {context}")

# 已知TIM10_IRQHandler会调用GPIO_ToggleBits
# 该函数会XOR端口的ODR寄存器
# PC4 = bit 4 = 0x10
# PC0 = bit 0 = 0x01

# 创建修改版本
print("\n创建手动修改说明...")
with open('PATCH_INSTRUCTIONS.txt', 'w') as f:
    f.write("手动修改方法：\n")
    f.write("================\n\n")
    f.write("1. 在hex编辑器中打开 original_working.hex\n")
    f.write("2. 搜索 GPIO_ToggleBits 的调用\n")
    f.write("3. 找到传递 0x10 (GPIO_Pin_4) 的地方\n")
    f.write("4. 改为 0x01 (GPIO_Pin_0)\n")
    f.write("\n")
    f.write("特征码：\n")
    f.write("- TIM10中断向量在中断表的某个位置\n")
    f.write("- 会调用 GPIOC 相关函数\n")
    f.write("- 使用 pin mask 0x0010\n")
    f.write("\n")
    f.write("或者：\n")
    f.write("让硬件工程师把LED接到PC4（如果该引脚存在）\n")

print("说明已生成: PATCH_INSTRUCTIONS.txt")