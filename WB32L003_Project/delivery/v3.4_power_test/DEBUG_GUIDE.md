# WB32L003 Power-On Debug Guide

## Current Problem
Device won't power on despite implementing what should be the correct transistor logic according to o3's analysis.

## Power Control Analysis Summary

### o3's Transistor Analysis (Should be Correct)
- **PC6**: Controls Q5 P-MOS via Q6 NPN → **LOW = ON, HIGH = OFF**
- **PA3**: Controls Q7 PNP directly → **LOW = ON, HIGH = OFF** 
- **PB1**: Controls Q7 PNP for backlight → **LOW = ON, HIGH = OFF**

### Why Logic is Inverted
1. **P-MOS (Q5)**: Gate needs LOW relative to source to conduct
2. **PNP (Q7)**: Base needs LOW relative to emitter to conduct
3. NPN (Q6) inverts PC6 signal before Q5

## Systematic Debug Approach

### 1. Basic Hardware Verification
- [ ] Verify USB power is reaching the board (measure VBUS)
- [ ] Check if any LEDs light up briefly during power-on
- [ ] Measure voltage on PC6 pin after programming
- [ ] Check if pressing the button (PB0) has any effect

### 2. Power Test Sequence
Run through all 16 combinations in `power_test_combinations.txt`:
- Combination 1: All LOW (o3's recommended)
- Combination 16: All HIGH (洪哥's document)
- Watch for any LED activity or screen flicker

### 3. Additional Tests

#### A. Edge-Triggered Power
Some power controllers need edge transitions:
```
PC6: LOW → HIGH → LOW (hold LOW)
```

#### B. Power Sequencing
Try different orders:
1. PC6 first, then PA3, then PB1
2. All at once
3. PA3 first (LCD), then PC6 (main)

#### C. Timing Issues
- Hold each state for different durations (100ms to 2s)
- Some caps may need time to charge

#### D. Missing Power Controls
Test these potentially undocumented pins:
- PD0, PD1 - Could be additional power controls
- PA0 - Listed as button but might have dual function

### 4. Measurement Points
If you have access to a multimeter:

| Pin | Expected (Power ON) | Notes |
|-----|-------------------|-------|
| PC6 | 0V | Controls main power |
| PA3 | 0V | LCD power enable |
| PB1 | 0V | Backlight enable |
| Q5 Gate | ~0V | P-MOS should conduct |
| LCD VDD | 3.3V | If LCD powered |

### 5. Common Issues to Check

1. **Wrong Flash Address**: Already fixed (0x00000000)
2. **Clock Not Starting**: Crystal/PLL issues
3. **Brown-out Reset**: Voltage too low
4. **Watchdog Reset**: Resetting before we set pins
5. **Missing Pull-ups**: Transistors floating

### 6. Minimal Test Firmware

The test firmware cycles through:
1. o3 logic (all LOW) - Red LED
2. 洪哥 logic (all HIGH) - Green LED  
3. Mixed states - Both LEDs
4. PD0/PD1 test - Toggle LEDs
5. PC6 pulse test

Each state lasts 2 seconds.

### 7. If Nothing Works

Consider:
1. **Hardware damage** from previous incorrect voltages
2. **Missing initialization** sequence we don't know about
3. **Bootloader** requirements (ISP mode?)
4. **Power supply** current limits

## Next Steps

1. Run through all 16 power combinations
2. Check for any visual feedback (LED, screen backlight)
3. Try holding the power button during different states
4. Measure voltages if possible
5. Report which test (if any) shows signs of life