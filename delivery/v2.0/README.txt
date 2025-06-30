耳放固件交付包 v1.0
==================

芯片方案：WB32L003K8U6
- ARM Cortex-M0+ 内核
- 64KB Flash
- 8KB RAM
- 工作电压：2.0V-5.5V

固件文件：
- firmware.bin (63.5KB) - 二进制格式，用于烧录工具
- firmware.hex (183KB) - Intel HEX格式，备选烧录格式

已实现功能：
1. ST7735 LCD驱动 (160x80分辨率)
2. VU表显示动画 (20fps)
3. 电池电量监测 (4格显示)
4. 低电压保护 (<3.0V闪烁LED，<2.8V自动关机)
5. 按键功能：
   - 短按切换 STEREO/MONO 模式
   - 长按3秒关机
6. 低功耗待机模式

烧录说明：
使用SWD接口连接烧录器，烧录地址：0x08000000

引脚连接：
- LCD_CS:  PA4
- LCD_RST: PA5
- LCD_DC:  PA6
- LCD_SCK: PA7
- LCD_SDA: PB0
- LCD_BL:  PA1 (PWM)
- BAT_ADC: PA0
- PWR_BTN: PA2
- PWR_LED: PA3

注意事项：
1. 首次上电需要按住电源键启动
2. STEREO/MONO模式切换后会立即生效
3. 电池电压低于2.8V时设备将自动关机保护电池