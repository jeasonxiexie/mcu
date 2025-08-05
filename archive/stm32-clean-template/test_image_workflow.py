#!/usr/bin/env python3
"""
Test script to demonstrate the image workflow

This script creates sample images and tests the conversion workflow
to verify everything works correctly.
"""

import os
import sys
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

def create_test_images():
    """Create sample test images to demonstrate the workflow"""
    
    images_dir = Path('assets/images')
    images_dir.mkdir(parents=True, exist_ok=True)
    
    # Create boot logo (80x160, vertical)
    boot_img = Image.new('RGB', (80, 160), color=(0, 100, 200))
    draw = ImageDraw.Draw(boot_img)
    
    # Draw some simple graphics
    draw.rectangle([10, 60, 70, 100], fill=(255, 255, 255))
    draw.text((25, 70), "BOOT", fill=(0, 0, 0))
    draw.text((20, 110), "LOGO", fill=(255, 255, 255))
    
    boot_img.save(images_dir / 'boot_logo.png')
    print("Created boot_logo.png (80x160)")
    
    # Create shutdown logo
    shutdown_img = Image.new('RGB', (80, 160), color=(200, 0, 0))
    draw = ImageDraw.Draw(shutdown_img)
    
    draw.rectangle([10, 60, 70, 100], fill=(255, 255, 255))
    draw.text((15, 70), "POWER", fill=(0, 0, 0))
    draw.text((25, 85), "OFF", fill=(0, 0, 0))
    
    shutdown_img.save(images_dir / 'shutdown_logo.png')
    print("Created shutdown_logo.png (80x160)")
    
    # Create simple VU meter animation frames
    frames = []
    for i in range(6):  # Create 6 frames for demo
        frame = Image.new('RGB', (80, 160), color=(0, 0, 0))
        draw = ImageDraw.Draw(frame)
        
        # Draw VU meter bars
        bar_height = 10 + i * 20
        left_bar_height = min(bar_height, 140)
        right_bar_height = min(bar_height + 10, 140)
        
        # Left channel (green to red gradient)
        for j in range(0, left_bar_height, 4):
            color_val = min(255, j * 2)
            color = (color_val, 255 - color_val, 0)
            draw.rectangle([10, 150-j, 25, 150-j-3], fill=color)
        
        # Right channel
        for j in range(0, right_bar_height, 4):
            color_val = min(255, j * 2)
            color = (color_val, 255 - color_val, 0)
            draw.rectangle([55, 150-j, 70, 150-j-3], fill=color)
        
        # Add labels
        draw.text((12, 10), "L", fill=(255, 255, 255))
        draw.text((57, 10), "R", fill=(255, 255, 255))
        draw.text((30, 25), f"F{i+1}", fill=(255, 255, 255))
        
        frames.append(frame)
    
    # Save as GIF animation
    frames[0].save(
        images_dir / 'meter_af.gif',
        save_all=True,
        append_images=frames[1:],
        duration=50,  # 50ms per frame = 20 FPS
        loop=0
    )
    print("Created meter_af.gif (6 frames, 20fps)")
    
    return True

def test_conversion():
    """Test the image conversion tools"""
    
    print("\n=== Testing Image Conversion ===")
    
    # Test single image conversion
    print("\n1. Testing single image conversion...")
    os.system("python tools/img2c.py assets/images/boot_logo.png")
    
    # Test GIF animation conversion  
    print("\n2. Testing GIF animation conversion...")
    os.system("python tools/img2c.py assets/images/meter_af.gif")
    
    # Test batch conversion
    print("\n3. Testing batch conversion with integration...")
    os.system("python tools/convert_assets.py --generate-integration")
    
    return True

def verify_output():
    """Verify that all expected files were generated"""
    
    print("\n=== Verifying Generated Files ===")
    
    generated_dir = Path('assets/generated')
    expected_files = [
        'boot_logo.h', 'boot_logo.c',
        'shutdown_logo.h', 'shutdown_logo.c', 
        'meter_af.h', 'meter_af.c'
    ]
    
    missing_files = []
    for filename in expected_files:
        filepath = generated_dir / filename
        if filepath.exists():
            size = filepath.stat().st_size
            print(f"✓ {filename} ({size} bytes)")
        else:
            missing_files.append(filename)
            print(f"✗ {filename} (missing)")
    
    # Check for additional generated files
    if (Path('assets') / 'resources.h').exists():
        print("✓ resources.h")
    else:
        print("✗ resources.h (missing)")
        
    if (Path('assets') / 'ui_integration_example.c').exists():
        print("✓ ui_integration_example.c")
    else:
        print("✗ ui_integration_example.c (missing)")
    
    if missing_files:
        print(f"\nWarning: {len(missing_files)} files are missing")
        return False
    else:
        print(f"\n✓ All expected files generated successfully!")
        return True

def show_usage_example():
    """Show example of how to use the generated code"""
    
    print("\n=== Usage Example ===")
    print("""
To use the generated image resources in your STM32 project:

1. Include the resources header:
   #include "assets/resources.h"

2. Display a static image:
   ST7735_DrawImage(0, 0, BOOT_LOGO_WIDTH, BOOT_LOGO_HEIGHT, boot_logo_data);

3. Animate the VU meter:
   static uint8_t frame = 0;
   ST7735_DrawImage(0, 0, METER_AF_WIDTH, METER_AF_HEIGHT, meter_af_frame_ptrs[frame]);
   frame = (frame + 1) % METER_AF_FRAMES;

4. Add the generated .c files to your Makefile:
   C_SOURCES += assets/generated/boot_logo.c
   C_SOURCES += assets/generated/shutdown_logo.c  
   C_SOURCES += assets/generated/meter_af.c

See assets/ui_integration_example.c for complete examples.
""")

def main():
    """Main test function"""
    
    print("STM32 Image Workflow Test")
    print("=" * 50)
    
    # Check if we have the required tools
    tools_dir = Path('tools')
    if not (tools_dir / 'img2c.py').exists():
        print("Error: img2c.py not found in tools/ directory")
        sys.exit(1)
    
    try:
        # Create test images
        print("Creating test images...")
        create_test_images()
        
        # Test conversion
        test_conversion()
        
        # Verify output
        success = verify_output()
        
        # Show usage
        show_usage_example()
        
        if success:
            print("\n🎉 Image workflow test completed successfully!")
            print("Your STM32 Clean Architecture template is ready to use.")
        else:
            print("\n❌ Some files were not generated correctly.")
            sys.exit(1)
            
    except Exception as e:
        print(f"Error during test: {e}")
        sys.exit(1)

if __name__ == '__main__':
    main()