
# specs.md — 0.96″ TFT Demo Board (WB32L003K8U6)

> **Purpose**  
> Provide a structured, machine‑readable specification of the demo PCB so that code‑generation agents (e.g. Claude Code) can automatically map firmware to hardware.

---

## 1. System Overview

| Block | Device(s) | Power Rail | Key Nets |
|-------|-----------|-----------|----------|
| MCU   | WB32L003K8U6 (QFN32) | 3.3 V | SPI1, UART2, TIM1, ADC, GPIO |
| Display | TFT096‑8P (LH096NTSDA5) | 3.3 V LEDA via BL driver | LCD_SCL, LCD_SDA, LCD_RS, LCD_RESET, LCD_CS |
| Back‑light | S8550 (PNP) high‑level, MCU PD7 low‑level | 3.3 V → LEDA | CLD_BL1, CLD_BL2 |
| Audio  | RC4580IDR dual op‑amp, 3.5 mm stereo jack | +VCC / VSS | AF_L, AF_R, CON_STEREO |
| Power control | Q5 (6402BAT), Q6 (9014) & discretes | VBATT / 5 V USB | CON_POW_CPU, CON_POW_LCD, CON_POW_RF |
| Charging | TP4056‑compatible | 5 V USB | CHRG, CHAR, DET |
| Status LEDs | L‑C175JRJGCT‑AM (dual colour) | 3.3 V | LED_RED, LED_GREEN |
| Keys / Switches | MODE key, SW_POW | 3.3 V pull‑ups | KEY_MODE, SW_POW |

---

## 2. Power Rails

| Rail | Nominal | Source | Notes |
|------|---------|--------|-------|
| +3.3 V | 3.3 V | LDO (U?) fed from VBATT/USB | Main digital & analogue rail |
| VBATT | 3.0–4.2 V | Li‑ion cell | Unregulated system supply |
| +5 V_USB | 5 V | USB‑C | Input for charging & operation |

---

## 3. MCU Pin Allocation (WB32L003K8U6)

| Pin# | Port.Pin | Net | Peripheral / Function |
|------|----------|-----|-----------------------|
| 17 | PC0 | LCD_CS | SPI1_NSS → TFT CS |
| 18 | PC1 | LCD_RS | GPIO → TFT D/C |
| 19 | PC2 | LCD_RESET | GPIO → TFT RESET |
| 20 | PC3 | — | Spare / TIM1_CH3 |
| 21 | PC4 | — | TIM1_CH4 / CLK_CCO |
| 22 | PC5 | LCD_SCL | SPI1_SCK |
| 23 | PC6 | LCD_SDA | SPI1_MOSI |
| 24 | PC7 | LCD_MISO (NC on TFT) | Optional input |
| 25 | PD0 | CLD_BL1 | Back‑light high‑level (active‑LOW) |
| 32 | PD7 | CLD_BL2 | Back‑light low‑level (active‑HIGH / PWM) |
| 29 | PD4 | BEEP | TIM2_CH1 |
| 30 | PD5 | UART2_TX | Debug / console |
| 31 | PD6 | UART2_RX | Debug / console |
| …  | …  | CON_POW_* | Power‑control outputs (see §4) |

---

## 4. Control Nets

| Net | Direction | Target | Description |
|-----|-----------|--------|-------------|
| CON_POW_CPU | MCU → Q5/Q6 | Main 3.3 V enable |
| CON_POW_LCD | MCU → S8550 | 3.3 V to LCD panel |
| CON_POW_RF  | MCU → RF section | Radio supply enable |
| KEY_MODE | KEY → MCU | Momentary user key |
| SW_POW | Slide SW → Q5/Q6 | Hardware power latch |
| MUTE | MCU → Audio amp | Low‑active mute control |

---

## 5. Display Sub‑system

### 5.1 TFT096‑8P Connector (J5/J6 CON8)

| Pin | Signal | MCU Net | Note |
|-----|--------|---------|------|
| 1 | LEDA | LEDA | Back‑light anode (3.3 V via driver) |
| 2 | GND | GND | Ground |
| 3 | RESET | LCD_RESET | Active‑LOW |
| 4 | RS (D/C) | LCD_RS | Data / Command |
| 5 | SDA | LCD_SDA | SPI1_MOSI |
| 6 | SCL | LCD_SCL | SPI1_SCK |
| 7 | VCC | 3.3 V | Panel power |
| 8 | CS | LCD_CS | Chip‑select |

### 5.2 Back‑light Driver

```
MCU PD0 (CLD_BL1) —— R63 4k7 ——|< S8550 PNP |—— LEDA —— 3.3 V
MCU PD7 (CLD_BL2) —— R16 220R —— LEDA (dim PWM)
```

- **High‑level (CLD_BL1)**: drive **LOW** for 100 % brightness (PNP sinks current).  
- **Low‑level (CLD_BL2)**: optional PWM for night mode (220 Ω current limit).

---

## 6. Audio

- Dual‑op‑amp **RC4580** conditions AF_L / AF_R.  
- `CON_STEREO` allows mono/stereo selection via signal `KEY_STEREO`.  
- `MUTE` net pulls low to silence output.

---

## 7. Battery & Charging

| Signal | Description |
|--------|-------------|
| DET | 5 V present detect (USB) |
| CHRG | Charging in‑progress (from charger IC) |
| CHAR | Charge complete |
| V_Batt | Battery voltage monitor (ADC) |

---

## 8. Connectors

| Ref | Type | Purpose |
|-----|------|---------|
| P1 | 5‑pin (1.27/2.0/2.54) | SWD/ISP (NRST, CK, D, 3V3, GND) |
| J5, J6 | CON8 0.5 mm FFC | LCD panel |
| J4 | 6.35 mm TRS | Headphone / Line out |
| J1, J3 | SIP‑5 2 mm | Audio flying leads |

---

## 9. Bill of Materials (critical parts)

| Ref | Value / Part | Footprint |
|-----|--------------|-----------|
| U6 | WB32L003K8U6 | QFN‑32 |
| U7 | TFT096‑8P (LH096NTSDA5) | 0.96″ IPS panel |
| Q7 | S8550 | SOT‑23 |
| R63 | 4.7 kΩ | 0603 |
| R16 | 220 Ω | 0603 |
| LED1 | L‑C175JRJGCT‑AM | 0606 bi‑colour |
| U1A/B | RC4580IDR | SOIC‑8 |

---

## 10. Firmware Hints

1. **SPI config**: Mode 0, ≤ 12 MHz; send `0x36, 0xA0` for address set.  
2. **Init sequence**: reset ≥ 10 ms, sleep out `0x11`, display on `0x29`.  
3. **Back‑light**: pull CLD_BL1 = 0, optional PD7 PWM 1 kHz.  
4. **Low‑power**: tie CLD_BL1 = 1, CLD_BL2 = 0, CON_POW_LCD = 0.

---

*Generated 2025-08-06*
