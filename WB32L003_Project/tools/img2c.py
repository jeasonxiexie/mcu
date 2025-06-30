#!/usr/bin/env python3
"""
Convert PNG/GIF images to C arrays for ST7735 display (RGB565 format)
"""

import os
import sys
from PIL import Image

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565"""
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def image_to_c_array(image_path, array_name, target_width=160, target_height=80):
    """Convert image to C array"""
    try:
        img = Image.open(image_path)
        
        # Convert to RGB if necessary
        if img.mode != 'RGB':
            img = img.convert('RGB')
        
        # Resize to target dimensions
        img = img.resize((target_width, target_height), Image.Resampling.LANCZOS)
        
        # Generate C array
        c_code = f"// Generated from {os.path.basename(image_path)}\n"
        c_code += f"// Size: {target_width}x{target_height}\n"
        c_code += f"const uint16_t {array_name}[{target_width*target_height}] = {{\n"
        
        pixels = []
        for y in range(target_height):
            for x in range(target_width):
                r, g, b = img.getpixel((x, y))
                rgb565 = rgb888_to_rgb565(r, g, b)
                pixels.append(f"0x{rgb565:04X}")
        
        # Format array nicely
        for i in range(0, len(pixels), 16):
            c_code += "    " + ", ".join(pixels[i:i+16])
            if i + 16 < len(pixels):
                c_code += ","
            c_code += "\n"
        
        c_code += "};\n"
        
        return c_code
        
    except Exception as e:
        print(f"Error processing {image_path}: {e}")
        return None

def gif_to_c_arrays(gif_path, array_prefix, target_width=160, target_height=80):
    """Convert GIF frames to C arrays"""
    try:
        gif = Image.open(gif_path)
        frames = []
        frame_count = 0
        
        c_code = f"// Generated from {os.path.basename(gif_path)}\n"
        c_code += f"// Size: {target_width}x{target_height}\n"
        
        try:
            while True:
                # Convert frame to RGB
                frame = gif.copy()
                if frame.mode != 'RGB':
                    frame = frame.convert('RGB')
                
                # Resize frame
                frame = frame.resize((target_width, target_height), Image.Resampling.LANCZOS)
                
                # Generate frame array
                frame_data = []
                for y in range(target_height):
                    for x in range(target_width):
                        r, g, b = frame.getpixel((x, y))
                        rgb565 = rgb888_to_rgb565(r, g, b)
                        frame_data.append(f"0x{rgb565:04X}")
                
                frames.append(frame_data)
                frame_count += 1
                gif.seek(frame_count)
                
        except EOFError:
            pass
        
        # Generate C code for all frames
        c_code += f"#define {array_prefix.upper()}_FRAME_COUNT {frame_count}\n\n"
        c_code += f"const uint16_t {array_prefix}_frames[{frame_count}][{target_width*target_height}] = {{\n"
        
        for frame_idx, frame_data in enumerate(frames):
            c_code += f"    // Frame {frame_idx}\n"
            c_code += "    {\n"
            for i in range(0, len(frame_data), 16):
                c_code += "        " + ", ".join(frame_data[i:i+16])
                if i + 16 < len(frame_data):
                    c_code += ","
                c_code += "\n"
            c_code += "    }"
            if frame_idx < len(frames) - 1:
                c_code += ","
            c_code += "\n"
        
        c_code += "};\n"
        
        return c_code, frame_count
        
    except Exception as e:
        print(f"Error processing {gif_path}: {e}")
        return None, 0

def main():
    if len(sys.argv) < 2:
        print("Usage: python img2c.py <image_file> [array_name]")
        sys.exit(1)
    
    image_path = sys.argv[1]
    
    if not os.path.exists(image_path):
        print(f"Error: File {image_path} not found")
        sys.exit(1)
    
    # Determine array name
    if len(sys.argv) >= 3:
        array_name = sys.argv[2]
    else:
        base_name = os.path.splitext(os.path.basename(image_path))[0]
        array_name = base_name.replace('-', '_').replace(' ', '_').lower()
    
    # Process based on file type
    ext = os.path.splitext(image_path)[1].lower()
    
    if ext == '.gif':
        c_code, frame_count = gif_to_c_arrays(image_path, array_name)
        if c_code:
            output_file = f"{array_name}.c"
            with open(output_file, 'w') as f:
                f.write(c_code)
            print(f"Generated {output_file} with {frame_count} frames")
    else:
        c_code = image_to_c_array(image_path, array_name)
        if c_code:
            output_file = f"{array_name}.c"
            with open(output_file, 'w') as f:
                f.write(c_code)
            print(f"Generated {output_file}")

if __name__ == "__main__":
    main()