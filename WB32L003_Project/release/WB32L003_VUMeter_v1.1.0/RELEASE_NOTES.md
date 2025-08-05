# WB32L003 VU Meter Firmware v1.1.0

## 🎉 新版本特性

这是一个重大更新版本，实现了真正的音频电平实时检测功能！

### ✨ 新增功能

1. **真实音频ADC采样**
   - 实现了左右声道独立ADC采样（PA2/PA3）
   - 采样率优化到适合VU表显示的频率
   - 峰值检测算法，确保捕捉瞬态信号

2. **完整的硬件控制**
   - 所有GPIO引脚已正确定义和初始化
   - 电源控制引脚（CON_POW, CON_LCD）
   - 音频控制引脚（MUTE, V2）
   - LED指示灯（绿色/红色）

3. **改进的MONO模式显示**
   - STEREO模式：左右声道分开显示
   - MONO模式：中央显示合并电平
   - 模式切换立即生效，无需重启

4. **优化的电平计算**
   - 去除DC偏置处理
   - 对数响应曲线，更符合人耳感知
   - 噪声门限，避免静音时误显示

### 🐛 修复的问题

- 修复了之前使用假数据的问题
- 修复了MONO模式显示逻辑
- 修复了引脚冲突问题（SPI与其他功能）

### 📊 内存使用

```
程序存储器 (Flash): 64,304 bytes (98% of 64KB)
数据存储器 (RAM):   1,996 bytes (24% of 8KB)
```

### 🔧 硬件连接

#### 音频输入
- L_AD (PA2) - 左声道音频检测
- R_AD (PA3) - 右声道音频检测

#### 控制引脚
- MUTE (PA4) - 音频静音控制（高电平=取消静音）
- V2 (PB6) - STEREO/MONO选择（高电平=MONO）
- CON_POW (PB7) - 主电源控制
- CON_LCD (PB8) - LCD电源控制

#### LED指示
- GREEN (PB3) - 绿色LED（低电平=亮）
- RED (PB4) - 红色LED（低电平=亮）

#### 其他
- CHRG (PB5) - 充电检测（低电平=充电中）
- KEY_PWR (PC13) - 电源按键

### 🚀 烧录方法

1. **使用J-Link/ST-Link烧录器**
   ```bash
   # 烧录.hex文件
   JLinkExe -device WB32L003K8U6 -if SWD -speed 4000 -CommanderScript flash.jlink
   
   # 或使用OpenOCD
   openocd -f interface/stlink.cfg -f target/wb32l003.cfg -c "program firmware.hex verify reset exit"
   ```

2. **使用串口ISP烧录**
   - 将BOOT0引脚拉高
   - 使用WB32L003专用ISP工具烧录.bin文件
   - 烧录完成后将BOOT0拉低

### ⚠️ 注意事项

1. **音频输入电路**
   - 需要在L_AD和R_AD引脚添加分压电路
   - 将音频信号范围调整到0-3.3V
   - 建议添加输入保护

2. **电源要求**
   - 电池电压：3.0V - 4.2V
   - 低于3.0V会闪烁红色LED警告
   - 低于2.8V会自动关机

3. **按键操作**
   - 短按：切换STEREO/MONO模式
   - 长按（>3秒）：关机

### 📝 版本历史

- v1.1.0 (2025-01-07) - 实现真实音频ADC采样
- v1.0.0 (2025-01-06) - 初始版本（使用演示数据）

### 🔗 相关资源

- 源代码：WB32L003_Project/
- 硬件原理图：请联系硬件工程师
- WB32L003数据手册：参考官方文档

---