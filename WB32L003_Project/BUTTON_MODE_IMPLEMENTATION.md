# Button Mode Switching Implementation

## Summary
按键切换 STEREO/MONO 模式功能已实现。

## Implementation Details

### Button Behavior
- **Short Press (<3 seconds)**: Toggle between STEREO and MONO mode
- **Long Press (>3 seconds)**: Power off

### Code Changes

1. **main.c** - Added mode switching logic:
```c
// If short press (< 3s), toggle audio mode
if (press_time < 3000 && press_time > 0)
{
    // Toggle between STEREO and MONO
    static audio_mode_t current_audio_mode = AUDIO_MODE_STEREO;
    current_audio_mode = (current_audio_mode == AUDIO_MODE_STEREO) ? 
                       AUDIO_MODE_MONO : AUDIO_MODE_STEREO;
    UI_SetAudioMode(current_audio_mode);
}
```

2. **ui.c** - Enhanced visual feedback:
- Clear the mode text area before drawing new mode
- MONO mode: Orange rectangle (0xFD20)
- STEREO mode: Cyan rectangle (0x07FF)
- Ready for actual text images when PNG assets are available

### Current Status
✅ Button press detection with debouncing (50ms)
✅ Short press toggles STEREO/MONO mode
✅ Long press (>3s) powers off device
✅ Visual feedback with colored rectangles
✅ Mode state is maintained in static variable

### TODO
1. **Save mode to non-volatile memory** - Currently commented out:
   ```c
   // TODO: Save audio mode to flash/EEPROM for power-off memory
   // SaveAudioModeToFlash(current_audio_mode);
   ```
   
2. **Load saved mode on startup** - Add to main() initialization:
   ```c
   // Load saved audio mode from flash
   audio_mode_t saved_mode = LoadAudioModeFromFlash();
   UI_SetAudioMode(saved_mode);
   ```

3. **Replace colored rectangles with actual text images** when PNGs are available

### Testing
The firmware now supports:
- Press power button briefly → Mode switches between STEREO/MONO
- Hold power button for 3+ seconds → Device powers off
- Visual indication shows current mode at bottom of display