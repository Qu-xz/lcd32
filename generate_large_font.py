#!/usr/bin/env python3
"""
生成20x40像素Roman风格数字字体
"""

def generate_digit_20x40(digit):
    """生成20x40像素的Roman风格数字字模"""
    font = [[0]*20 for _ in range(40)]
    
    def draw_h_line(y, x1, x2):
        for x in range(x1, x2+1):
            if 0 <= x < 20 and 0 <= y < 40:
                font[y][x] = 1
    
    def draw_v_line(x, y1, y2):
        for y in range(y1, y2+1):
            if 0 <= x < 20 and 0 <= y < 40:
                font[y][x] = 1
    
    # Roman风格数字 - 优雅的衬线体
    if digit == 0:
        # 0: 椭圆形
        draw_h_line(2, 6, 13)
        draw_h_line(3, 5, 14)
        draw_h_line(36, 6, 13)
        draw_h_line(37, 5, 14)
        draw_v_line(3, 4, 35)
        draw_v_line(4, 3, 36)
        draw_v_line(15, 4, 35)
        draw_v_line(16, 3, 36)
        
    elif digit == 1:
        # 1: 带底座和顶部装饰
        draw_v_line(9, 2, 4)
        draw_v_line(10, 3, 5)
        draw_v_line(10, 6, 37)
        draw_v_line(11, 6, 37)
        draw_h_line(36, 6, 15)
        draw_h_line(37, 5, 16)
        
    elif digit == 2:
        # 2: 优雅的曲线
        draw_h_line(2, 5, 14)
        draw_h_line(3, 4, 15)
        draw_v_line(15, 4, 8)
        draw_v_line(16, 5, 9)
        draw_h_line(10, 6, 14)
        draw_h_line(11, 5, 15)
        draw_v_line(4, 28, 36)
        draw_v_line(5, 27, 37)
        draw_h_line(36, 5, 14)
        draw_h_line(37, 4, 15)
        
    elif digit == 3:
        # 3: 圆润的曲线
        draw_h_line(2, 5, 14)
        draw_h_line(3, 4, 15)
        draw_v_line(15, 4, 8)
        draw_v_line(16, 5, 9)
        draw_h_line(10, 6, 14)
        draw_h_line(11, 5, 15)
        draw_v_line(15, 30, 35)
        draw_v_line(16, 29, 36)
        draw_h_line(36, 5, 14)
        draw_h_line(37, 4, 15)
        
    elif digit == 4:
        # 4: 斜线交叉
        draw_v_line(4, 2, 20)
        draw_v_line(5, 2, 20)
        draw_h_line(20, 5, 16)
        draw_h_line(21, 4, 17)
        draw_v_line(12, 2, 37)
        draw_v_line(13, 2, 37)
        
    elif digit == 5:
        # 5: 优雅的S形
        draw_h_line(2, 4, 15)
        draw_h_line(3, 3, 16)
        draw_v_line(4, 4, 10)
        draw_v_line(5, 3, 11)
        draw_h_line(10, 5, 14)
        draw_h_line(11, 5, 15)
        draw_v_line(15, 28, 36)
        draw_v_line(16, 27, 37)
        draw_h_line(36, 5, 14)
        draw_h_line(37, 4, 15)
        
    elif digit == 6:
        # 6: 圆润的形状
        draw_h_line(2, 6, 14)
        draw_h_line(3, 5, 15)
        draw_v_line(4, 4, 36)
        draw_v_line(5, 3, 37)
        draw_h_line(20, 5, 14)
        draw_h_line(21, 5, 15)
        draw_v_line(15, 28, 36)
        draw_v_line(16, 27, 37)
        draw_h_line(36, 6, 14)
        draw_h_line(37, 5, 15)
        
    elif digit == 7:
        # 7: 优雅的斜线
        draw_h_line(2, 4, 15)
        draw_h_line(3, 3, 16)
        draw_v_line(4, 4, 8)
        draw_v_line(5, 3, 9)
        draw_v_line(10, 10, 37)
        draw_v_line(11, 10, 37)
        
    elif digit == 8:
        # 8: 上下两个圆
        draw_h_line(2, 6, 13)
        draw_h_line(3, 5, 14)
        draw_v_line(4, 4, 9)
        draw_v_line(5, 3, 10)
        draw_v_line(14, 4, 9)
        draw_v_line(15, 3, 10)
        draw_h_line(10, 6, 13)
        draw_h_line(11, 5, 14)
        draw_v_line(4, 30, 35)
        draw_v_line(5, 29, 36)
        draw_v_line(14, 30, 35)
        draw_v_line(15, 29, 36)
        draw_h_line(36, 6, 13)
        draw_h_line(37, 5, 14)
        
    elif digit == 9:
        # 9: 上圆下开
        draw_h_line(2, 6, 13)
        draw_h_line(3, 5, 14)
        draw_v_line(4, 4, 18)
        draw_v_line(5, 3, 19)
        draw_v_line(14, 4, 18)
        draw_v_line(15, 3, 19)
        draw_h_line(20, 6, 13)
        draw_h_line(21, 5, 14)
        draw_v_line(15, 30, 36)
        draw_v_line(16, 29, 37)
        draw_h_line(36, 6, 13)
        draw_h_line(37, 5, 14)
    
    return font

def generate_degree_symbol():
    """生成°符号 (12x12像素，放在右上角)"""
    font = [[0]*20 for _ in range(40)]
    
    # °符号 - 小圆环，放在顶部
    cx, cy, r = 9, 6, 4
    
    for y in range(12):
        for x in range(20):
            dist = ((x - cx) ** 2 + (y - cy) ** 2) ** 0.5
            # 圆环
            if 2.5 <= dist <= 4.5:
                font[y][x] = 1
    
    return font

def font_to_bytes_120(font):
    """将20x40字体数组转换为120字节格式（每行3字节）"""
    bytes_data = []
    
    for row in range(40):
        byte1 = 0
        byte2 = 0
        byte3 = 0
        
        for col in range(20):
            if font[row][col]:
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

def generate_c_array():
    """生成C语言数组"""
    print("// 20x40 Roman风格数字字体数据 (每个字符120字节)")
    print("static const uint8_t font_20x40_nums[][120] = {")
    
    for digit in range(10):
        font = generate_digit_20x40(digit)
        bytes_data = font_to_bytes_120(font)
        
        print(f"    // {digit}")
        print("    {")
        
        for i in range(0, len(bytes_data), 12):
            line_bytes = bytes_data[i:i+12]
            hex_str = ", ".join(f"0x{b:02X}" for b in line_bytes)
            if i + 12 < len(bytes_data):
                print(f"        {hex_str},")
            else:
                print(f"        {hex_str}")
        
        print("    },")
    
    # °符号
    print("    // ° (degree symbol)")
    font = generate_degree_symbol()
    bytes_data = font_to_bytes_120(font)
    print("    {")
    for i in range(0, len(bytes_data), 12):
        line_bytes = bytes_data[i:i+12]
        hex_str = ", ".join(f"0x{b:02X}" for b in line_bytes)
        if i + 12 < len(bytes_data):
            print(f"        {hex_str},")
        else:
            print(f"        {hex_str}")
    print("    }")
    
    print("};")

if __name__ == "__main__":
    generate_c_array()