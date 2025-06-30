#!/usr/bin/env python3
"""
Generate UI assets C arrays for ST7735 display
This script creates the actual C code to be inserted into ui_assets.c
"""

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565"""
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def generate_background_base():
    """Generate background with gradient effect"""
    width, height = 160, 80
    pixels = []
    
    print("Generating background base (160x80)...")
    
    # Create a dark blue gradient background
    for y in range(height):
        for x in range(width):
            # Gradient from dark blue to lighter blue
            b = min(255, 20 + (y * 2))
            rgb565 = rgb888_to_rgb565(0, 0, b)
            pixels.append(f"0x{rgb565:04X}")
    
    # Format as C array
    c_code = "// Background base image (160x80 pixels)\n"
    c_code += "// Dark blue gradient background\n"
    c_code += "const uint16_t ui_background_base[UI_DISPLAY_WIDTH * UI_DISPLAY_HEIGHT] = {\n"
    
    for i in range(0, len(pixels), 16):
        c_code += "    " + ", ".join(pixels[i:i+16])
        if i + 16 < len(pixels):
            c_code += ","
        c_code += "\n"
    
    c_code += "};\n"
    return c_code

def generate_battery_empty():
    """Generate battery empty icon"""
    width, height = 20, 10
    pixels = []
    
    print("Generating battery empty icon (20x10)...")
    
    # Create battery outline
    for y in range(height):
        for x in range(width):
            # Battery outline logic
            if x < 18:  # Main battery body
                if y == 0 or y == 9 or x == 0 or x == 17:
                    pixels.append("0xFFFF")  # White outline
                else:
                    pixels.append("0x0000")  # Black inside
            else:  # Battery terminal
                if 3 <= y <= 6:
                    pixels.append("0xFFFF")  # White terminal
                else:
                    pixels.append("0x0000")  # Black
    
    # Format as C array
    c_code = "// Battery empty icon (20x10 pixels)\n"
    c_code += "const uint16_t ui_battery_empty[BATTERY_ICON_WIDTH * BATTERY_ICON_HEIGHT] = {\n"
    
    for i in range(0, len(pixels), 10):
        c_code += "    " + ", ".join(pixels[i:i+10])
        if i + 10 < len(pixels):
            c_code += ","
        c_code += "\n"
    
    c_code += "};\n"
    return c_code

def generate_battery_bar():
    """Generate battery bar (green fill)"""
    width, height = 16, 6
    green_rgb565 = rgb888_to_rgb565(0, 255, 0)
    
    print("Generating battery bar (16x6)...")
    
    c_code = "// Battery bar (16x6 pixels) - green fill for battery level\n"
    c_code += "const uint16_t ui_battery_bar[BATTERY_BAR_WIDTH * BATTERY_BAR_HEIGHT] = {\n"
    c_code += f"    [0 ... (BATTERY_BAR_WIDTH * BATTERY_BAR_HEIGHT - 1)] = 0x{green_rgb565:04X}\n"
    c_code += "};\n"
    return c_code

def generate_mono_text():
    """Generate MONO text bitmap"""
    width, height = 40, 10
    
    print("Generating MONO text (40x10)...")
    
    # Simple bitmap for MONO text (simplified representation)
    c_code = "// MONO text (40x10 pixels)\n"
    c_code += "const uint16_t ui_mono_text[MONO_TEXT_WIDTH * MONO_TEXT_HEIGHT] = {\n"
    
    # Create a simple pattern for MONO text
    pixels = []
    for y in range(height):
        for x in range(width):
            # Simple text pattern (would be replaced with actual font data)
            if 2 <= y <= 7 and ((5 <= x <= 8) or (12 <= x <= 15) or (19 <= x <= 22) or (26 <= x <= 29)):
                pixels.append("0xFFFF")  # White pixels for text
            else:
                pixels.append("0x0000")  # Black background
    
    for i in range(0, len(pixels), 10):
        c_code += "    " + ", ".join(pixels[i:i+10])
        if i + 10 < len(pixels):
            c_code += ","
        c_code += "\n"
    
    c_code += "};\n"
    return c_code

def generate_stereo_text():
    """Generate STEREO text bitmap"""
    width, height = 50, 10
    
    print("Generating STEREO text (50x10)...")
    
    c_code = "// STEREO text (50x10 pixels)\n"
    c_code += "const uint16_t ui_stereo_text[STEREO_TEXT_WIDTH * STEREO_TEXT_HEIGHT] = {\n"
    
    # Create a simple pattern for STEREO text
    pixels = []
    for y in range(height):
        for x in range(width):
            # Simple text pattern (would be replaced with actual font data)
            if 2 <= y <= 7 and ((3 <= x <= 6) or (9 <= x <= 12) or (15 <= x <= 18) or 
                                (21 <= x <= 24) or (27 <= x <= 30) or (33 <= x <= 36) or (39 <= x <= 42)):
                pixels.append("0xFFFF")  # White pixels for text
            else:
                pixels.append("0x0000")  # Black background
    
    for i in range(0, len(pixels), 10):
        c_code += "    " + ", ".join(pixels[i:i+10])
        if i + 10 < len(pixels):
            c_code += ","
        c_code += "\n"
    
    c_code += "};\n"
    return c_code

def generate_volume_blocks():
    """Generate all 32 volume blocks with gradient colors"""
    width, height = 5, 40
    
    print("Generating 32 volume blocks (5x40 each)...")
    
    c_code = "// Volume level blocks (5x40 pixels each)\n"
    c_code += "// Gradient colors from green to yellow to red for VU meter\n\n"
    
    # Define color gradients
    colors = [
        (1, 16, (0, 255, 0)),      # Green
        (17, 24, (255, 255, 0)),   # Yellow
        (25, 28, (255, 165, 0)),   # Orange
        (29, 32, (255, 0, 0))      # Red
    ]
    
    for block_num in range(1, 33):
        # Determine color for this block
        color = None
        for start, end, rgb in colors:
            if start <= block_num <= end:
                color = rgb
                break
        
        rgb565 = rgb888_to_rgb565(*color)
        
        c_code += f"// Volume block {block_num:02d}\n"
        c_code += f"const uint16_t ui_volume_block_{block_num:02d}[VOLUME_BLOCK_WIDTH * VOLUME_BLOCK_HEIGHT] = {{\n"
        
        # Create gradient effect within each block
        pixels = []
        for y in range(height):
            for x in range(width):
                # Add slight gradient within the block
                r, g, b = color
                brightness = 1.0 - (y / height) * 0.3  # Darker at bottom
                r = int(r * brightness)
                g = int(g * brightness)
                b = int(b * brightness)
                rgb565 = rgb888_to_rgb565(r, g, b)
                pixels.append(f"0x{rgb565:04X}")
        
        for i in range(0, len(pixels), 10):
            c_code += "    " + ", ".join(pixels[i:i+10])
            if i + 10 < len(pixels):
                c_code += ","
            c_code += "\n"
        
        c_code += "};\n\n"
    
    # Add array of pointers
    c_code += "// Array of pointers to volume blocks for easy indexed access\n"
    c_code += "const uint16_t* ui_volume_blocks[VOLUME_BLOCKS_COUNT] = {\n"
    
    for i in range(0, 32, 4):
        c_code += "    "
        blocks = []
        for j in range(4):
            if i + j < 32:
                blocks.append(f"ui_volume_block_{i+j+1:02d}")
        c_code += ", ".join(blocks)
        if i + 4 < 32:
            c_code += ","
        c_code += "\n"
    
    c_code += "};\n"
    
    return c_code

def main():
    print("Generating UI assets C code...\n")
    
    # Generate all assets
    all_code = '#include "ui_assets.h"\n\n'
    
    all_code += generate_background_base() + "\n"
    all_code += generate_battery_empty() + "\n"
    all_code += generate_battery_bar() + "\n"
    all_code += generate_mono_text() + "\n"
    all_code += generate_stereo_text() + "\n"
    all_code += generate_volume_blocks()
    
    # Save to file
    output_file = "ui_assets_generated.c"
    with open(output_file, 'w') as f:
        f.write(all_code)
    
    print(f"\nGenerated {output_file}")
    print("You can now copy the contents to Core/Src/ui_assets.c")

if __name__ == "__main__":
    main()