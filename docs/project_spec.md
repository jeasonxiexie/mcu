# project_spec.md
## MCU
- Model: WB32L003K8U6, 32-bit Cortex-M0+, 64 KB Flash, 8 KB RAM
## Pin Map
| Function | Port | Notes |
| -------- | ---- | ----- |
| TFT_SCLK | PA5  | SPI1_SCK |
| TFT_MOSI | PA7  | SPI1_MOSI |
| TFT_CS   | PB0  | Chip-Select |
| TFT_DC   | PB1  | Data/Command |
| TFT_RST  | PB2  | Hardware reset |
| BL_EN    | PA1  | PWM 背光 |
| BAT_ADC  | PA0  | 电池电量检测 |
| KEY_PWR  | PC13 | 关机键 |
## Display
- Panel: 0.96" TFT ST7735-S, SPI, 160×80, RGB565
- 背光三挡：5 mA / 12 mA / 20 mA
## UI Assets
- boot_logo.png 160×80、16-bit
- off_logo.png  160×80、16-bit
- meter_af.gif  12 frames, 20 fps
## Power Policy
- < 3.0 V：红 LED 闪
- < 2.8 V：自动关机
...
