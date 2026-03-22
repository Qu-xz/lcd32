#!/usr/bin/env python3
"""
从系统TTF字体生成20x40像素数字字体数据
"""

from PIL import Image, ImageDraw, ImageFont
import os

def generate_font_data(font_path, font_size, chars, output_width, output_height):
    """从TTF字体生成位图数据"""
    
    try:
        font = ImageFont.truetype(font_path, font_size)
    except Exception as e:
        print(f"无法加载字体 {font_path}: {e}")
        return None
    
    font_data = {}
    
    for char in chars:
        # 创建更大的图像用于绘制
        img = Image.new('L', (output_width * 4, output_height * 4), 0)
        draw = ImageDraw.Draw(img)
        
        # 获取字符边界框
        bbox = draw.textbbox((0, 0), char, font=font)
        char_width = bbox[2] - bbox[0]
        char_height = bbox[3] - bbox[1]
        
        # 居中绘制字符
        x = (output_width * 4 - char_width) // 2 - bbox[0]
        y = (output_height * 4 - char_height) // 2 - bbox[1]
        
        draw.text((x, y), char, font=font, fill=255)
        
        # 裁剪到字符区域
        img = img.crop((x + bbox[0] - 4, y + bbox[1] - 4, 
                        x + bbox[2] + 4, y + bbox[3] + 4))
        
        # 调整到目标大小
        img = img.resize((output_width, output_height), Image.Resampling.LANCZOS)
        
        # 增强对比度
        img = img.point(lambda p: 255 if p > 100 else 0)
        
        # 转换为位图
        bitmap = []
        for py in range(output_height):
            row = []
            for px in range(output_width):
                pixel = img.getpixel((px, py))
                row.append(1 if pixel > 128 else 0)
            bitmap.append(row)
        
        font_data[char] = bitmap
    
    return font_data

def bitmap_to_bytes(bitmap, width, height):
    """将位图转换为字节数组（每行3字节，20位）"""
    bytes_data = []
    
    for row in range(height):
        byte1 = 0  # 位 0-7
        byte2 = 0  # 位 8-15
        byte3 = 0  # 位 16-19
        
        for col in range(width):
            if bitmap[row][col]:
                if col < 8:
                    byte1 |= (1 << (7 - col))
                elif col < 16:
                    byte2 |= (1 << (7 - (col - 8)))
                else:
                    byte3 |= (1 << (7 - (col - 16)))
        
        bytes_data.append(byte1)
        bytes_data.append(byte2)
        bytes_data.append(byte3)
    
    return bytes_data

def generate_c_header(font_data, output_width, output_height, output_file):
    """生成C语言头文件"""
    
    with open(output_file, 'w') as f:
        f.write("// 从系统字体生成的20x40数字字体数据\n")
        f.write("// 每个字符120字节（每行3字节）\n\n")
        f.write("#ifndef FONT_20X40_H\n")
        f.write("#define FONT_20X40_H\n\n")
        f.write("#include <stdint.h>\n\n")
        f.write(f"// 字体尺寸: {output_width}x{output_height}\n")
        f.write(f"// 每个字符字节数: 120\n\n")
        f.write("static const uint8_t font_20x40_nums[][120] = {\n")
        
        # 数字 0-9
        for digit in range(10):
            char = str(digit)
            if char in font_data:
                bitmap = font_data[char]
                bytes_data = bitmap_to_bytes(bitmap, output_width, output_height)
                
                f.write(f"    // {digit}\n")
                f.write("    {\n")
                
                for i in range(0, len(bytes_data), 12):
                    line_bytes = bytes_data[i:i+12]
                    hex_str = ", ".join(f"0x{b:02X}" for b in line_bytes)
                    if i + 12 < len(bytes_data):
                        f.write(f"        {hex_str},\n")
                    else:
                        f.write(f"        {hex_str}\n")
                
                f.write("    },\n")
        
        # °符号
        if 'degree' in font_data:
            bitmap = font_data['degree']
            bytes_data = bitmap_to_bytes(bitmap, output_width, output_height)
            
            f.write("    // ° (degree symbol)\n")
            f.write("    {\n")
            
            for i in range(0, len(bytes_data), 12):
                line_bytes = bytes_data[i:i+12]
                hex_str = ", ".join(f"0x{b:02X}" for b in line_bytes)
                if i + 12 < len(bytes_data):
                    f.write(f"        {hex_str},\n")
                else:
                    f.write(f"        {hex_str}\n")
            
            f.write("    }\n")
        
        f.write("};\n\n")
        f.write("#endif // FONT_20X40_H\n")

def main():
    # 字体路径 - 使用Helvetica
    font_path = "/System/Library/Fonts/Helvetica.ttc"
    
    if not os.path.exists(font_path):
        print("未找到字体!")
        return
    
    print(f"使用字体: {font_path}")
    
    # 参数设置 - 更大的字体
    font_size = 48  # 更大的字体
    output_width = 20
    output_height = 40
    
    # 要生成的字符
    chars = [str(d) for d in range(10)]
    
    # 生成字体数据
    print("正在生成字体数据...")
    font_data = generate_font_data(font_path, font_size, chars, output_width, output_height)
    
    if font_data:
        # 生成°符号（使用较小的字体）
        print("正在生成°符号...")
        degree_font = ImageFont.truetype(font_path, 24)
        img = Image.new('L', (output_width * 4, output_height * 4), 0)
        draw = ImageDraw.Draw(img)
        
        # 绘制°符号
        bbox = draw.textbbox((0, 0), "°", font=degree_font)
        x = (output_width * 4 - (bbox[2] - bbox[0])) // 2 - bbox[0]
        y = (output_width * 4 - (bbox[3] - bbox[1])) // 2 - bbox[1]
        draw.text((x, y), "°", font=degree_font, fill=255)
        
        # 裁剪并调整大小
        img = img.crop((x + bbox[0] - 4, y + bbox[1] - 4, 
                        x + bbox[2] + 4, y + bbox[3] + 4))
        img = img.resize((output_width, output_height), Image.Resampling.LANCZOS)
        img = img.point(lambda p: 255 if p > 100 else 0)
        
        # 转换为位图
        degree_bitmap = []
        for py in range(output_height):
            row = []
            for px in range(output_width):
                pixel = img.getpixel((px, py))
                row.append(1 if pixel > 128 else 0)
            degree_bitmap.append(row)
        
        font_data['degree'] = degree_bitmap
        
        # 生成头文件
        output_file = "/Users/flyarmy/pico/mypicoproject/lcd32/font_20x40.h"
        print(f"正在生成头文件: {output_file}")
        generate_c_header(font_data, output_width, output_height, output_file)
        print("完成!")

if __name__ == "__main__":
    main()