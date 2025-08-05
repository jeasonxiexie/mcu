# Claude.md

### ⬆️ 上级: [../Claude.md](../Claude.md)

## 🧠 模块意图
> 这一层负责 实现所有核心功能，包括主程序、LCD驱动、UI渲染、音频处理、电源管理和各种LCD变体

## 📋 待办任务
---
id: SRC-001
path: ./main.c
status: Ready
---
- [ ] SRC-001 优化主程序入口实现   # status: Ready

---
id: SRC-002
path: ./audio_adc.c
status: Ready
---
- [ ] SRC-002 实现音频ADC处理功能   # status: Ready

---
id: SRC-003
path: ./early_power.c
status: Ready
---
- [ ] SRC-003 维护早期电源初始化   # status: Ready

---
id: SRC-004
path: ./hardware_control.c
status: Ready
---
- [ ] SRC-004 优化硬件控制层实现   # status: Ready

---
id: SRC-005
path: ./hardware_control_fixed.c
status: Ready
---
- [ ] SRC-005 修复版硬件控制实现   # status: Ready

---
id: SRC-006
path: ./power.c
status: Ready
---
- [ ] SRC-006 实现电源管理功能   # status: Ready

---
id: SRC-007
path: ./screen_test.c
status: Ready
---
- [ ] SRC-007 屏幕测试功能实现   # status: Ready

---
id: SRC-008
path: ./st7735.c
status: Ready
---
- [ ] SRC-008 ST7735S LCD驱动核心实现   # status: Ready

---
id: SRC-009
path: ./syscalls.c
status: Ready
---
- [ ] SRC-009 系统调用桩函数实现   # status: Ready

---
id: SRC-010
path: ./ui.c
status: Ready
---
- [ ] SRC-010 用户界面渲染引擎实现   # status: Ready

---
id: SRC-011
path: ./ui_assets.c
status: Ready
---
- [ ] SRC-011 UI资源数据实现   # status: Ready

---
id: SRC-012
path: ./wb32l003_it.c
status: Ready
---
- [ ] SRC-012 中断处理程序实现   # status: Ready

---
id: SRC-013
path: ./lcd_v1_no_offset.c
status: Ready
---
- [ ] SRC-013 LCD无偏移变体实现   # status: Ready

---
id: SRC-014
path: ./lcd_v2_offset_24.c
status: Ready
---
- [ ] SRC-014 LCD 24像素偏移变体   # status: Ready

---
id: SRC-015
path: ./lcd_v3_offset_26.c
status: Ready
---
- [ ] SRC-015 LCD 26像素偏移变体   # status: Ready

---
id: SRC-016
path: ./lcd_v4_offset_32.c
status: Ready
---
- [ ] SRC-016 LCD 32像素偏移变体   # status: Ready

---
id: SRC-017
path: ./lcd_mvp.c
status: Ready
---
- [ ] SRC-017 LCD最小可行产品实现   # status: Ready

---
id: SRC-018
path: ./lcd_diagnostic.c
status: Ready
---
- [ ] SRC-018 LCD诊断功能实现   # status: Ready