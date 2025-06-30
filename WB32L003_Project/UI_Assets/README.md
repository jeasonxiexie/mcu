# UI Assets Directory

This directory should contain the PNG images for the UI elements.

## Required PNG Files

Place the following PNG files in this directory:

1. **基地素材图.png** - Base background image (160x80 pixels)
2. **电量空.png** - Empty battery icon (20x10 pixels)  
3. **电量绿条.png** - Battery fill bar (16x6 pixels)
4. **MONO字.png** - MONO text (40x10 pixels)
5. **STEREO字.png** - STEREO text (50x10 pixels)
6. **色块_01.png** through **色块_32.png** - Volume level blocks (5x40 pixels each)

## Converting Images to C Arrays

### Method 1: Using the conversion script
```bash
cd /Users/jeasonseven/Documents/workspace/BensProject/WB32L003_Project
./convert_png_assets.sh
```

### Method 2: Manual conversion
```bash
# Convert individual images
python3 tools/img2c.py UI_Assets/基地素材图.png ui_background_base
python3 tools/img2c.py UI_Assets/电量空.png ui_battery_empty
python3 tools/img2c.py UI_Assets/电量绿条.png ui_battery_bar
python3 tools/img2c.py UI_Assets/MONO字.png ui_mono_text
python3 tools/img2c.py UI_Assets/STEREO字.png ui_stereo_text

# Convert volume blocks
for i in {01..32}; do
    python3 tools/img2c.py UI_Assets/色块_${i}.png ui_volume_block_${i}
done
```

### Method 3: Using the generate_ui_assets.py script (for testing without PNGs)
```bash
python3 generate_ui_assets.py
```

## Current Status

Currently, the ui_assets.c file contains generated test data with:
- Dark blue gradient background
- Battery icon outline with proper terminal
- Green battery fill bar
- Simple text patterns for MONO/STEREO
- Volume blocks with proper color gradients (green→yellow→orange→red)

When you have the actual PNG files, use the conversion script to generate the real image data.

## Image Format Notes

- All images are converted to RGB565 format (16-bit color)
- The img2c.py tool automatically resizes images to the expected dimensions
- Transparency is not supported (converted to black)
- Images are stored as const arrays in flash memory