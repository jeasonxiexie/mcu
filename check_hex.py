#!/usr/bin/env python3
# 检查 hex 文件中是否有异常的地址

with open('build/firmware.hex', 'r') as f:
    lines = f.readlines()

base_addr = 0
problem_lines = []

for i, line in enumerate(lines):
    line = line.strip()
    if not line.startswith(':'):
        continue
    
    record_type = int(line[7:9], 16)
    
    if record_type == 4:  # 扩展地址
        high_addr = int(line[9:13], 16)
        base_addr = high_addr << 16
        if base_addr != 0x08000000:
            problem_lines.append(f'Line {i+1}: Extended address = 0x{base_addr:08X}')
    
    elif record_type == 0:  # 数据记录
        address = int(line[3:7], 16)
        actual_addr = base_addr + address
        if actual_addr >= 0x08010000:  # 超过 64KB
            problem_lines.append(f'Line {i+1}: Address 0x{actual_addr:08X} exceeds 64KB')
            break

if problem_lines:
    print('发现问题:')
    for p in problem_lines:
        print(p)
else:
    print('未发现地址越界问题')

# 检查特殊记录
for i, line in enumerate(lines):
    if line.startswith(':04000005'):
        print(f'\n第 {i+1} 行发现起始地址记录: {line.strip()}')
        data = line[9:17]
        start_addr = int(data, 16)
        print(f'起始地址: 0x{start_addr:08X}')