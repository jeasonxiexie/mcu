#!/usr/bin/env python3
"""
清理 Intel HEX 文件，移除可能导致烧录软件兼容性问题的记录
"""

import sys

def clean_hex_file(input_file, output_file):
    """
    清理 HEX 文件，只保留数据记录和 EOF 记录
    移除：
    - 扩展线性地址记录 (类型 04)
    - 起始地址记录 (类型 05)
    - 扩展段地址记录 (类型 02)
    """
    with open(input_file, 'r') as f_in, open(output_file, 'w') as f_out:
        for line in f_in:
            if line.startswith(':'):
                # 获取记录类型（第8-9个字符）
                record_type = line[7:9]
                
                # 只保留数据记录(00)和EOF记录(01)
                if record_type in ['00', '01']:
                    f_out.write(line)
                elif record_type == '04':
                    # 如果是必要的扩展地址记录（0x0800），保留它
                    if '0800' in line:
                        f_out.write(line)
                    else:
                        print(f"移除扩展线性地址记录: {line.strip()}")
                elif record_type == '05':
                    print(f"移除起始地址记录: {line.strip()}")
                elif record_type == '02':
                    print(f"移除扩展段地址记录: {line.strip()}")
                else:
                    print(f"移除未知记录类型 {record_type}: {line.strip()}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("用法: python3 clean_hex.py <输入文件> <输出文件>")
        sys.exit(1)
    
    clean_hex_file(sys.argv[1], sys.argv[2])
    print(f"清理完成: {sys.argv[2]}")