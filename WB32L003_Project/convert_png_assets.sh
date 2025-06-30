#!/bin/bash
# Script to convert PNG images to C arrays using img2c.py
# This script shows how to use the img2c.py tool when you have actual PNG files

echo "Converting PNG assets to C arrays..."

# Create output directory
mkdir -p generated_assets

# Convert background image (160x80)
if [ -f "UI_Assets/基地素材图.png" ]; then
    python3 tools/img2c.py UI_Assets/基地素材图.png ui_background_base
    echo "✓ Converted background base image"
else
    echo "⚠ Background image not found: UI_Assets/基地素材图.png"
fi

# Convert battery icons
if [ -f "UI_Assets/电量空.png" ]; then
    python3 tools/img2c.py UI_Assets/电量空.png ui_battery_empty
    echo "✓ Converted battery empty icon"
else
    echo "⚠ Battery empty icon not found: UI_Assets/电量空.png"
fi

if [ -f "UI_Assets/电量绿条.png" ]; then
    python3 tools/img2c.py UI_Assets/电量绿条.png ui_battery_bar
    echo "✓ Converted battery bar"
else
    echo "⚠ Battery bar not found: UI_Assets/电量绿条.png"
fi

# Convert text images
if [ -f "UI_Assets/MONO字.png" ]; then
    python3 tools/img2c.py UI_Assets/MONO字.png ui_mono_text
    echo "✓ Converted MONO text"
else
    echo "⚠ MONO text not found: UI_Assets/MONO字.png"
fi

if [ -f "UI_Assets/STEREO字.png" ]; then
    python3 tools/img2c.py UI_Assets/STEREO字.png ui_stereo_text
    echo "✓ Converted STEREO text"
else
    echo "⚠ STEREO text not found: UI_Assets/STEREO字.png"
fi

# Convert volume blocks (1-32)
for i in $(seq -f "%02g" 1 32); do
    if [ -f "UI_Assets/色块_${i}.png" ]; then
        python3 tools/img2c.py UI_Assets/色块_${i}.png ui_volume_block_${i}
        echo "✓ Converted volume block ${i}"
    else
        echo "⚠ Volume block not found: UI_Assets/色块_${i}.png"
    fi
done

echo ""
echo "Conversion complete!"
echo "Generated C files are in the current directory."
echo ""
echo "To update ui_assets.c:"
echo "1. Open each generated .c file"
echo "2. Copy the array contents"
echo "3. Replace the corresponding placeholder arrays in Core/Src/ui_assets.c"
echo ""
echo "Note: The img2c.py tool automatically:"
echo "- Converts images to RGB565 format"
echo "- Resizes images to the expected dimensions"
echo "- Generates properly formatted C arrays"