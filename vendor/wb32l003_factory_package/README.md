# WB32L003 项目源代码包

## 包含内容

### 1. wb32l003_full_project/
完整的VU表项目源代码：
- 完整功能实现
- 根据原理图更新的引脚定义（LCD连接在GPIOC）
- 80×160 LCD配置
- 包含ui_assets.c（界面资源）
- **注意**: build/firmware.hex 是更新前的版本，需要重新编译

### 2. wb32l003_lcd_tests/
LCD渐进式测试程序（含源码和固件）：
- **test_v1_backlight**: 背光测试
- **test_v2_lcd_reset**: LCD复位测试
- **test_v3_full_display**: 完整显示测试

每个测试都包含：
- main.c 源代码
- .hex 编译好的固件
- 详细说明文档

### 3. previous_test_firmwares/
历史测试固件，包含不同配置的测试版本

### 4. documents/
项目相关文档：
- 硬件分析
- 调试记录
- 引脚映射

## 快速开始

### 测试LCD显示
1. 先烧录 `wb32l003_lcd_tests/test_v1_backlight/test_v1_backlight_only.hex`
2. 观察LED是否闪烁，背光是否亮起
3. 如果成功，继续测试v2和v3

### 关键信息
- **LCD型号**: LH096NTSDA5 / ZTB096TB
- **分辨率**: 80×160
- **MCU**: WB32L003 (ARM Cortex-M0+)
- **LCD引脚连接** (根据原理图分析):
  - PC0: LCD_SCL (SPI时钟)
  - PC1: LCD_SDA (SPI数据)
  - PC3: LCD_RESET
  - PC4: LCD_RS (数据/命令)
  - PC6: LCD_CS (片选)

## 已知问题

### 主要问题
程序启动后仅运行极短时间（LED闪一次即停止），LCD始终无显示。

### 可能原因
1. WB32L003特有的初始化要求
2. 时钟配置问题
3. 看门狗或其他保护机制
4. 需要特定的启动代码

## 调试建议

1. **使用调试器**
   - 推荐J-Link或ST-Link
   - 单步调试查看程序停止位置

2. **检查基础配置**
   - 系统时钟
   - 看门狗设置
   - 电源管理（PC6引脚）

3. **参考原厂资料**
   - WB32L003 SDK
   - 官方示例代码
   - 启动代码模板

## 技术支持需求

建议向WB32原厂获取：
1. 完整的启动代码（startup_wb32l003.s）
2. 系统时钟配置示例
3. 看门狗禁用方法
4. ISP烧录工具使用说明

## 联系方式
如有技术问题，请参考WB32官方技术支持。

## 重要提示

⚠️ **源代码已更新**：根据原理图分析，LCD引脚定义已更新为GPIOC。包内的.hex文件是更新前的版本，请使用更新后的源代码重新编译测试。

### 编译要求
- ARM GCC工具链 (arm-none-eabi-gcc)
- 在各测试目录下使用提供的Makefile编译

---
*注：本代码包已根据原理图分析进行了引脚修正。*