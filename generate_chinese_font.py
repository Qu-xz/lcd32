#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
中文字模提取工具
支持从TrueType字体文件中提取指定汉字的字模数据
"""

import os
import sys
from PIL import Image, ImageDraw, ImageFont

class ChineseFontGenerator:
    def __init__(self, font_path, font_size=16):
        """
        初始化字体生成器

        Args:
            font_path: 字体文件路径（.ttf或.otf）
            font_size: 字体大小（像素）
        """
        self.font_path = font_path
        self.font_size = font_size
        self.font = ImageFont.truetype(font_path, font_size)

    def extract_glyph(self, char):
        """
        提取单个汉字的字模数据

        Args:
            char: 汉字字符

        Returns:
            字模数据（字节数组）
        """
        # 创建图像
        img = Image.new('1', (self.font_size, self.font_size), 0)
        draw = ImageDraw.Draw(img)

        # 绘制字符
        draw.text((0, 0), char, font=self.font, fill=1)

        # 转换为字模数据（逐行式，每行2字节）
        glyph_data = []
        for y in range(self.font_size):
            # 第一字节：前8个像素
            byte1 = 0
            for x in range(8):
                if img.getpixel((x, y)):
                    byte1 |= (1 << (7 - x))
            glyph_data.append(byte1)

            # 第二字节：后8个像素
            byte2 = 0
            for x in range(8, 16):
                if img.getpixel((x, y)):
                    byte2 |= (1 << (7 - (x - 8)))
            glyph_data.append(byte2)

        return bytes(glyph_data)

    def generate_font_data(self, chars):
        """
        生成字库数据

        Args:
            chars: 汉字列表

        Returns:
            字库数据和字符映射表
        """
        glyph_data = []
        char_map = ""

        for char in chars:
            glyph = self.extract_glyph(char)
            glyph_data.extend(glyph)
            char_map += char

        return bytes(glyph_data), char_map

    def generate_c_code(self, chars, font_name="chinese_font"):
        """
        生成C语言代码

        Args:
            chars: 汉字列表
            font_name: 字体名称

        Returns:
            C语言代码字符串
        """
        glyph_data, char_map = self.generate_font_data(chars)

        # 生成字模数据数组
        c_code = f"// {font_name} - {len(chars)}个汉字\n"
        c_code += f"// 字体大小: {self.font_size}x{self.font_size}\n"
        c_code += f"// 字体文件: {os.path.basename(self.font_path)}\n\n"
        c_code += f"static const uint8_t {font_name}_glyphs[] = {{\n"

        # 每行16个字节
        for i in range(0, len(glyph_data), 16):
            line_data = glyph_data[i:i+16]
            hex_str = ", ".join(f"0x{b:02X}" for b in line_data)
            c_code += f"    {hex_str},\n"

        c_code += "};\n\n"

        # 生成字符映射表
        c_code += f"static const char {font_name}_char_map[] = \"{char_map}\";\n\n"

        # 生成字体结构
        c_code += f"const chinese_font_t {font_name} = {{\n"
        c_code += f"    {font_name}_glyphs,\n"
        c_code += f"    {self.font_size},   // width\n"
        c_code += f"    {self.font_size},   // height\n"
        c_code += f"    {len(chars)},   // char_count\n"
        c_code += f"    {font_name}_char_map\n"
        c_code += "};\n"

        return c_code

def generate_weather_font():
    """生成天气相关汉字字库"""
    # 常用系统字体路径
    font_paths = [
        "/System/Library/Fonts/PingFang.ttc",  # macOS
        "/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf",  # Linux
        "C:\\Windows\\Fonts\\msyh.ttc",  # Windows
        "C:\\Windows\\Fonts\\simhei.ttf",  # Windows
    ]

    # 查找可用的字体文件
    font_path = None
    for path in font_paths:
        if os.path.exists(path):
            font_path = path
            break

    if not font_path:
        print("错误：未找到中文字体文件")
        print("请手动指定字体文件路径")
        return

    print(f"使用字体: {font_path}")

    # 天气相关汉字
    weather_chars = [
        "北", "京", "天", "气", "实", "时", "温", "度",
        "晴", "阴", "雨", "雪", "多", "云", "雾", "霾",
        "风", "雷", "电", "暴", "大", "小", "中", "高",
        "低", "冷", "热", "干", "湿", "强", "弱", "无"
    ]

    # 生成16x16字库
    generator = ChineseFontGenerator(font_path, 16)
    c_code = generator.generate_c_code(weather_chars, "chinese_font_16x16")

    # 保存到文件
    output_file = "chinese_font_16x16.c"
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(c_code)

    print(f"字库已生成: {output_file}")
    print(f"包含 {len(weather_chars)} 个汉字")

def generate_gb2312_font():
    """生成GB2312常用汉字字库"""
    font_paths = [
        "/System/Library/Fonts/PingFang.ttc",
        "/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf",
        "C:\\Windows\\Fonts\\msyh.ttc",
        "C:\\Windows\\Fonts\\simhei.ttf",
    ]

    font_path = None
    for path in font_paths:
        if os.path.exists(path):
            font_path = path
            break

    if not font_path:
        print("错误：未找到中文字体文件")
        return

    print(f"使用字体: {font_path}")

    # GB2312常用汉字（一级字库，约3755个）
    # 这里只生成前100个常用汉字作为示例
    common_chars = [
        "的一是在不了有和人这中大为上个国我以要他时来用们生到作地于出就分对成会可主发年动同工也能下过子说产种面而方后多定行学法所民得经十三之进着等部度家电力里如水化高自二理起小物现实量都两体制机当使点从业本去把性好应开它合还因由其些然前外天政四日那社义事平形相全表间样与关各重新线内数正心反你明看原又么利比或但质气第向道命此变条只没结解问意建月公无系军很情者最立代想已通并提直题党程展五果料象员革位入常文总次品式活设及管特件长求老头基资边流路级少图山统接知较将组见计别她手角期根论运农指几九区强放决西被干做必战先回则任取据处队南给色光门即保治北造百规热领七海口东导器压志世金增争济阶油思术极交受联什认六共权收证改张象完却究支群市音强讲",
        "德音查类半满始速织张状容市省科视兰马坚效切华防史皇径轮喜层止边清卫远队衣略足般予列青飞确包读短神戏境击非鱼亚显顶独院刚才恩平好必战先回则任取据处队南给色光门即保治北造百规热领七海口东导器压志世金增争济阶油思术极交受联什认六共权收证改张象完却究支群市音强讲"
    ]

    # 合并所有字符并去重
    all_chars = []
    seen = set()
    for char_str in common_chars:
        for char in char_str:
            if char not in seen:
                seen.add(char)
                all_chars.append(char)

    print(f"生成 {len(all_chars)} 个常用汉字...")

    # 生成16x16字库
    generator = ChineseFontGenerator(font_path, 16)
    c_code = generator.generate_c_code(all_chars, "chinese_font_gb2312")

    # 保存到文件
    output_file = "chinese_font_gb2312.c"
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(c_code)

    print(f"字库已生成: {output_file}")
    print(f"包含 {len(all_chars)} 个常用汉字")

def main():
    """主函数"""
    print("中文字模提取工具")
    print("=" * 50)
    print("1. 生成天气相关汉字字库（16x16）")
    print("2. 生成GB2312常用汉字字库（16x16）")
    print("3. 自定义字库")
    print("=" * 50)

    choice = input("请选择 (1/2/3): ").strip()

    if choice == "1":
        generate_weather_font()
    elif choice == "2":
        generate_gb2312_font()
    elif choice == "3":
        font_path = input("请输入字体文件路径: ").strip()
        chars = input("请输入要提取的汉字: ").strip()
        char_list = list(chars)  # 直接遍历字符串

        if os.path.exists(font_path):
            generator = ChineseFontGenerator(font_path, 16)
            c_code = generator.generate_c_code(char_list, "chinese_font_custom")

            output_file = "chinese_font_custom.c"
            with open(output_file, "w", encoding="utf-8") as f:
                f.write(c_code)

            print(f"字库已生成: {output_file}")
            print(f"包含 {len(char_list)} 个汉字")
        else:
            print("错误：字体文件不存在")
    else:
        print("无效选择")

if __name__ == "__main__":
    main()