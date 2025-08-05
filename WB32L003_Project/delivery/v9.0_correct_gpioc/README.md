# v9.0 - 正确的 GPIOC LCD 测试固件

## 关键修正

### 错误的代码版本（v8.0 失败原因）
之前的 `lcd_test_fixed.c` 文件中，LCD 引脚被错误地定义到了 **GPIOB**：

```c
// 错误！LCD 不在 GPIOB 上！
#define LCD_CS_PIN      GPIO_PIN_0      // PB0 - CS
#define LCD_CS_PORT     GPIOB
```

### 正确的引脚映射（根据原理图）
LCD 所有控制引脚都在 **GPIOC**：

| 信号 | 引脚 | GPIO端口 |
|------|------|----------|
| LCD_CS | PC0 | GPIOC |
| LCD_RS/DC | PC1 | GPIOC |
| LCD_RST | PC3 | GPIOC |
| LCD_SCL | PC5 | GPIOC |
| LCD_SDA | PC6 | GPIOC |

背光控制在 GPIOB：
- CLD_BL1 = PB1 (PNP晶体管，低电平导通)
- CLD_BL2 = PB2 (PNP晶体管，低电平导通)

LCD 电源控制：
- CON_POW_LCD = PA3 (PNP晶体管，低电平导通)

## v9.0 版本特性

1. **使用正确的 pin_config.h**：引用项目统一的引脚定义
2. **正确的 GPIO 初始化**：LCD 引脚初始化到 GPIOC
3. **完整的 LCD 初始化序列**：包含 ST7735S 完整初始化
4. **X轴偏移补偿**：默认使用 26 像素偏移（京东方常见值）
5. **优化的刷屏效率**：减少不必要的 CS 切换

## 测试方法

烧录后，屏幕应该循环显示：
- 红色 (1秒)
- 绿色 (1秒)
- 蓝色 (1秒)
- 白色 (1秒)
- 黑色 (1秒)

## 如果仍然不亮

1. **检查硬件连接**：确保 LCD 模块正确连接
2. **测量电源**：PA3 应为低电平（LCD 供电）
3. **检查背光**：PB1/PB2 应为低电平
4. **调整偏移**：如果显示偏移，可能需要调整 x_offset 值

## 编译信息

- 编译时间：2025-08-04
- 代码大小：2600 bytes
- 基于：main_lcd_test_v9.c