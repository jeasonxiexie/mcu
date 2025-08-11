给硬件工程师的Keil编译说明
============================

最保守方案 - 只改LED引脚
------------------------

文件：main_minimal_change.c

改动内容（只有2处）：
1. 第29行：GPIO_Pin_4 改为 GPIO_Pin_0
2. 第109行：GPIO_Pin_4 改为 GPIO_Pin_0

其他所有代码与原TIM10_Count完全相同！

Keil编译步骤：
1. 打开原TIM10_Count工程
2. 用main_minimal_change.c替换原main.c
3. 编译
4. 生成的hex应该与原版大小相近（约8KB）

预期结果：
- PC6继续输出100kHz PWM（与原版相同）
- PC0会切换高低电平（原版是PC4）
- 如果PC0连接了LED（低电平有效），LED会闪烁

这是最安全的方案：
- 保留所有原始初始化
- 保留所有库函数调用
- 只改变GPIO引脚号