# 故障排查指南

## 问题1：MCU完全没有反应

### 症状
- LED完全不亮
- 串口没有任何输出
- 电源指示灯熄灭

### 检查项目

#### 1.1 电源锁存问题（最可能）
**测量点：** PC6引脚（或config.h中配置的POWER_LATCH引脚）

- [ ] 测量电压：______V （应该是3.3V）
- [ ] 如果是0V，说明电源锁存失败

**解决方法：**
1. 打开 `source/config.h`
2. 修改 `POWER_LATCH_PORT` 和 `POWER_LATCH_PIN`
3. 可能的值：
   - PC6（RTF文档中的v2配置）
   - PB7（某些版本可能使用）
   - PC5（其他可能）
4. 重新编译（双击build.bat）

#### 1.2 电源锁存极性错误
- [ ] 尝试修改 `POWER_LATCH_LEVEL`
  - 当前是1（高电平保持）
  - 改为0试试（低电平保持）

---

## 问题2：MCU运行但LED不闪

### 症状
- 串口有输出（看到"WB32L003 Test Program"）
- 但LED不闪烁

### 检查项目

#### 2.1 LED引脚配置错误
**测量点：** PC0和PC1（或config.h中配置的LED引脚）

- [ ] 用万用表测量LED引脚电压
- [ ] 电压应该在0V和3.3V之间切换

**解决方法：**
1. 确认实际LED连接的引脚
2. 修改 `config.h` 中的：
   - `LED1_PORT` 和 `LED1_PIN`
   - `LED2_PORT` 和 `LED2_PIN`

#### 2.2 LED极性错误
- [ ] 当前配置：`LED_ACTIVE_LOW = 1`（低电平亮）
- [ ] 如果LED是高电平亮，改为0

---

## 问题3：串口没有输出

### 症状
- LED可能在闪
- 但串口没有任何信息

### 检查项目

#### 3.1 串口连接
- [ ] PA9连接到USB转串口的RX
- [ ] GND连接正确
- [ ] 波特率设置为115200

#### 3.2 串口引脚错误
- [ ] 确认TX引脚是PA9
- [ ] 如果不是，修改config.h中的UART_TX_PORT和UART_TX_PIN

---

## 测量清单（请填写）

请使用万用表测量以下引脚的电压，并记录：

| 引脚 | 预期电压 | 实测电压 | 备注 |
|------|----------|----------|------|
| PC6  | 3.3V（稳定） | ______V | 电源锁存 |
| PC0  | 0-3.3V（切换） | ______V | LED1 |
| PC1  | 0-3.3V（切换） | ______V | LED2 |
| PA9  | 有串口信号 | _______ | UART TX |
| VDD  | 3.3V | ______V | 电源 |
| GND  | 0V | ______V | 地 |

---

## 快速诊断流程

```
开始
  ↓
PC6是3.3V吗？ ──否──> 修改POWER_LATCH配置
  ↓是
串口有输出吗？ ──否──> 检查PA9连接/波特率
  ↓是
LED在闪吗？ ────否──> 修改LED配置
  ↓是
一切正常！
```

---

## 常见配置组合

### 配置1（RTF文档v2版本）
```c
POWER_LATCH: PC6 (HIGH)
LED1: PC0 (LOW active)
LED2: PC1 (LOW active)
UART: PA9
```

### 配置2（可能的变体）
```c
POWER_LATCH: PB7 (HIGH)
LED1: PB0 (LOW active)
LED2: PB1 (LOW active)
UART: PA9
```

### 配置3（其他可能）
```c
POWER_LATCH: PC5 (HIGH)
LED1: PA0 (HIGH active)
LED2: PA1 (HIGH active)
UART: PA9
```

---

## 需要帮助？

如果按照以上步骤仍无法解决，请提供：
1. 填写完整的测量清单
2. config.h文件的内容
3. 串口输出的截图（如果有）
4. LED实际连接的原理图或照片