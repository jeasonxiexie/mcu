# UI Assets Conversion Guide

This guide explains how to convert your PNG images to C arrays for the ST7735 display.

## Required PNG Files

Place the following PNG files in a `resources/` directory:

1. **基地素材图.png** - Base background image (160x80 pixels)
2. **电量空.png** - Empty battery icon (20x10 pixels)
3. **电量绿条.png** - Battery fill bar (16x6 pixels)
4. **MONO字.png** - MONO text (40x10 pixels)
5. **STEREO字.png** - STEREO text (50x10 pixels)
6. **色块_01.png** through **色块_32.png** - Volume level blocks (5x40 pixels each)

## Converting Images to C Arrays

### 1. Install Required Dependencies

```bash
pip install pillow
```

### 2. Convert Individual Images

Use the `img2c.py` tool to convert each image:

```bash
# Create output directory
mkdir -p generated_assets

# Convert background image
python tools/img2c.py resources/基地素材图.png ui_background_base

# Convert battery icons
python tools/img2c.py resources/电量空.png ui_battery_empty
python tools/img2c.py resources/电量绿条.png ui_battery_bar

# Convert text images
python tools/img2c.py resources/MONO字.png ui_mono_text
python tools/img2c.py resources/STEREO字.png ui_stereo_text

# Convert volume blocks
for i in {01..32}; do
    python tools/img2c.py resources/色块_${i}.png ui_volume_block_${i}
done
```

### 3. Update ui_assets.c

After running the conversion scripts, you'll have generated C files. Copy the array contents from these files into the corresponding arrays in `Core/Src/ui_assets.c`.

## Image Format Requirements

- **Format**: PNG (other formats will be converted automatically)
- **Color Mode**: RGB (will be converted to RGB565)
- **Dimensions**: Should match the defined sizes in ui_assets.h
- **Transparency**: Not supported (will be converted to black)

## RGB565 Color Format

The ST7735 display uses RGB565 format:
- Red: 5 bits (bits 15-11)
- Green: 6 bits (bits 10-5)
- Blue: 5 bits (bits 4-0)

Example color values:
- Black: 0x0000
- White: 0xFFFF
- Red: 0xF800
- Green: 0x07E0
- Blue: 0x001F

## Customizing Volume Blocks

The volume blocks should have a gradient from green to red:
- Blocks 1-16: Green (low volume)
- Blocks 17-24: Yellow (medium volume)
- Blocks 25-28: Orange (high volume)
- Blocks 29-32: Red (peak volume)

## Memory Optimization

The current implementation stores all images in flash memory. For a production system with limited flash:

1. **Compress repeated patterns**: If volume blocks have similar patterns, store only unique patterns
2. **Use RLE compression**: For images with large areas of the same color
3. **Store only differences**: For animation frames, store only the changed pixels
4. **Use 1-bit images**: For text and icons that only need two colors

## Testing

After updating the assets, test the display:

1. Build and flash the firmware
2. Check that all images display correctly
3. Verify colors match expectations
4. Test battery animation at different levels
5. Test VU meter with various audio levels