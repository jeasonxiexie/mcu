# 电源控制修复版本 v2.2

## 修复内容

1. **P-MOSFET 控制逻辑修正**
   - PB7 (P_CTL) 现在输出 LOW 电平来开启电源
   - PB6 (CON_LCD) 现在输出 LOW 电平来开启LCD电源
   - 根据硬件团队确认：PB7=0 → Q5 开 → 外设上 3 V3

2. **初始化顺序优化**
   - GPIO初始化后立即设置电源引脚为LOW
   - 等待100ms确保电源稳定
   - 然后再初始化其他外设

## 关键代码修改

### hardware_control.c
```c
void HW_SetMainPower(bool enable)
{
    // P-MOSFET control: LOW = Power ON, HIGH = Power OFF
    HAL_GPIO_WritePin(CON_POW_PORT, CON_POW_PIN, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
```

### main.c
```c
HAL_Init();
SystemClock_Config();
GPIO_Init();

// Enable power immediately after GPIO init - CRITICAL!
// P-MOSFET requires LOW level to turn ON
HAL_GPIO_WritePin(CON_POW_PORT, CON_POW_PIN, GPIO_PIN_RESET);  // Power ON
HAL_GPIO_WritePin(CON_LCD_PORT, CON_LCD_PIN, GPIO_PIN_RESET);  // LCD Power ON
HAL_Delay(100);  // Wait for power to stabilize
```

## 测试步骤

1. 使用 WB-Link PRO Configurator v1.0.15 烧录 firmware_power_fixed.hex
2. 断开烧录器，插入 USB-C 供电
3. 系统应该自动开机，屏幕点亮显示启动画面

## 文件说明

- firmware_power_fixed.hex - 修复后的固件（Intel HEX格式）
- firmware_power_fixed.bin - 修复后的固件（二进制格式）
- 固件大小：64580 bytes

## 注意事项

如果屏幕仍然不亮，请检查：
1. USB-C 供电是否正常（应该有3.3V）
2. PB7 引脚电压（应该是0V表示电源开启）
3. 屏幕排线连接是否正常