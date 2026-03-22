# 中文字库建立指南

## 目录
1. [概述](#概述)
2. [方法一：使用Python工具自动生成](#方法一使用python工具自动生成)
3. [方法二：使用字模提取工具手动生成](#方法二使用字模提取工具手动生成)
4. [方法三：使用完整GB2312字库](#方法三使用完整gb2312字库)
5. [在项目中使用中文字库](#在项目中使用中文字库)
6. [常见问题](#常见问题)

---

## 概述

中文字库的建立主要有三种方式：

1. **指定字符**：只提取需要的汉字，节省Flash空间
2. **GB2312常用字**：提取GB2312一级字库（约3755个常用汉字）
3. **完整GB2312**：包含所有GB2312字符（约6763个字符）

### 字体大小选择

| 字体大小 | 单个汉字大小 | 100个汉字 | 1000个汉字 | 适用场景 |
|---------|------------|----------|-----------|---------|
| 12x12   | 24字节     | 2.4KB    | 24KB      | 小屏幕 |
| 16x16   | 32字节     | 3.2KB    | 32KB      | 推荐 |
| 24x24   | 72字节     | 7.2KB    | 72KB      | 大屏幕 |

---

## 方法一：使用Python工具自动生成

### 1. 安装依赖

```bash
pip install pillow
```

### 2. 运行工具

```bash
python3 generate_chinese_font.py
```

### 3. 选择选项

```
中文字模提取工具
==================================================
1. 生成天气相关汉字字库（16x16）
2. 生成GB2312常用汉字字库（16x16）
3. 自定义字库
==================================================
请选择 (1/2/3):
```

### 4. 选项说明

#### 选项1：天气相关汉字
自动生成以下汉字的字库：
```
北京天气实时温度晴阴雨雪多云雾雾霾风雷电暴大小中高低冷干湿强弱无
```

#### 选项2：GB2312常用汉字
生成约200个常用汉字的字库。

#### 选项3：自定义字库
可以指定任意汉字：
```
请输入字体文件路径: /System/Library/Fonts/PingFang.ttc
请输入要提取的汉字（用空格分隔）: 你好世界
```

### 5. 输出文件

工具会生成C语言代码文件，例如：
- `chinese_font_16x16.c` - 天气字库
- `chinese_font_gb2312.c` - GB2312字库
- `chinese_font_custom.c` - 自定义字库

---

## 方法二：使用字模提取工具手动生成

### Windows用户：PCtoLCD2002

#### 1. 下载安装
下载地址：http://www.cnblogs.com/mr-co/p/6412627.html

#### 2. 设置参数
```
模式：字符模式
字体：选择你想要的字体（如宋体、黑体、微软雅黑）
字宽：16
字高：16
取模方式：阴码+逐行式+顺向（高位在前）
输出格式：C语言数组
```

#### 3. 输入汉字
在"输入字符"框中输入需要的汉字，例如：
```
北京天气实时温度
```

#### 4. 生成字模
点击"生成字模"按钮，复制生成的C语言数组。

#### 5. 整合到项目
将生成的数组复制到 `chinese_font.c` 文件中。

### Linux/Mac用户：Image2Lcd

#### 1. 下载安装
```bash
git clone https://github.com/kingst/Image2Lcd.git
cd Image2Lcd
make
```

#### 2. 使用方法
```bash
./Image2Lcd -f "北京天气" -s 16 -o output.c
```

---

## 方法三：使用完整GB2312字库

### 1. 下载GB2312字库文件

可以从以下地址下载：
- https://github.com/notro/fbcp-ili9341/blob/master/fonts/gb2312.c
- https://github.com/adafruit/Adafruit-GFX-Library/blob/master/fontconvert/makefonts.sh

### 2. 字库大小

- **GB2312一级字库**：3755个汉字 × 32字节 = 120KB
- **GB2312完整字库**：6763个汉字 × 32字节 = 216KB

### 3. 注意事项

⚠️ **警告**：完整GB2312字库会占用大量Flash空间，Pico W的Flash只有2MB，建议使用指定字符的方式。

---

## 在项目中使用中文字库

### 1. 添加文件到项目

将生成的字库文件添加到项目中：
```
lcd32/
├── chinese_font.h
├── chinese_font.c
├── chinese_font_16x16.c  # 生成的字库文件
└── main.c
```

### 2. 修改CMakeLists.txt

```cmake
add_executable(lcd32
    main.c
    lcd32_driver.c
    u8g2_font.c
    chinese_font.c
    chinese_font_16x16.c  # 添加生成的字库文件
    wifi_config.c
    weather_api.c
)
```

### 3. 在代码中使用

#### 示例1：显示单个汉字

```c
#include "chinese_font.h"

// 显示"北"字
chinese_draw_char(10, 10, "北", &chinese_font_16x16, RGB565(255, 255, 255), RGB565(0, 0, 0));
```

#### 示例2：显示中文字符串

```c
// 显示"北京天气"
chinese_draw_string(10, 10, "北京天气", &chinese_font_16x16, RGB565(255, 255, 255), RGB565(0, 0, 0));
```

#### 示例3：居中显示

```c
// 居中显示"北京天气"
chinese_draw_string_centered(0, 0, LCD_WIDTH, LCD_HEIGHT, "北京天气", &chinese_font_16x16, RGB565(255, 255, 255), RGB565(0, 0, 0));
```

#### 示例4：混合显示中英文

```c
#include "u8g2_font.h"
#include "chinese_font.h"

// 显示"北京天气 10°C"
chinese_draw_string(10, 10, "北京天气", &chinese_font_16x16, RGB565(255, 255, 255), RGB565(0, 0, 0));
u8g2_draw_string(10 + 4 * 16, 10, " 10 C", &u8g2_font_8x13_tf, RGB565(255, 255, 255), RGB565(0, 0, 0));
```

### 4. 修改天气显示

修改 `main.c` 中的 `display_weather()` 函数：

```c
void display_weather(weather_data_t *weather) {
    // 清屏
    lcd_fill_screen(RGB565(20, 30, 60));

    // 标题 - 中文
    chinese_draw_string_centered(0, 10, LCD_WIDTH, 40, "北京天气", &chinese_font_16x16, RGB565(255, 255, 255), RGB565(20, 30, 60));

    // 分隔线
    lcd_draw_rect(10, 55, LCD_WIDTH - 20, 2, RGB565(100, 150, 255));

    // 城市名称
    chinese_draw_string_centered(0, 70, LCD_WIDTH, 30, "北京", &chinese_font_16x16, RGB565(255, 255, 0), RGB565(20, 30, 60));

    // 天气描述 - 中文
    chinese_draw_string_centered(0, 110, LCD_WIDTH, 30, weather->weather_text, &chinese_font_16x16, RGB565(200, 200, 200), RGB565(20, 30, 60));

    // 温度显示
    char temp_str[32];
    snprintf(temp_str, sizeof(temp_str), "%d C", weather->temperature);
    u8g2_draw_string_centered(0, 160, LCD_WIDTH, 50, temp_str, &u8g2_font_10x20_tf, RGB565(255, 100, 100), RGB565(20, 30, 60));

    // 底部信息 - 中文
    chinese_draw_string_centered(0, 280, LCD_WIDTH, 20, "实时天气", &chinese_font_16x16, RGB565(150, 150, 150), RGB565(20, 30, 60));
}
```

---

## 常见问题

### Q1: 字符显示为乱码？

**原因**：字符不在字库中

**解决**：
1. 确保字符已添加到字库中
2. 检查字符映射表是否正确
3. 使用Python工具重新生成字库

### Q2: 字体显示方向不对？

**原因**：字模提取方式不匹配

**解决**：
1. 检查取模方式是否为"逐行式+顺向"
2. 修改 `chinese_draw_char()` 函数中的位操作
3. 尝试翻转x坐标或y坐标

### Q3: Flash空间不足？

**原因**：字库太大

**解决**：
1. 只提取需要的汉字
2. 使用更小的字体（12x12）
3. 将字库存储到外部Flash

### Q4: 如何添加新汉字？

**解决**：
1. 使用Python工具重新生成字库
2. 在 `chinese_char_map` 中添加新字符
3. 添加对应的字模数据

### Q5: 如何使用自定义字体？

**解决**：
1. 准备TrueType字体文件（.ttf或.otf）
2. 使用Python工具指定字体路径
3. 生成字库文件

---

## 字体资源

### 系统字体路径

**macOS**:
```
/System/Library/Fonts/PingFang.ttc
/System/Library/Fonts/STHeiti Light.ttc
```

**Linux**:
```
/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf
/usr/share/fonts/truetype/wqy/wqy-microhei.ttc
```

**Windows**:
```
C:\Windows\Fonts\msyh.ttc
C:\Windows\Fonts\simhei.ttf
C:\Windows\Fonts\simsun.ttc
```

### 免费字体

- **思源黑体**：https://github.com/adobe-fonts/source-han-sans
- **文泉驿**：http://wenq.org/
- **站酷字体系列**：https://www.zcool.com.cn/special/zcoolfonts

---

## 示例代码

### 完整示例

```c
#include "chinese_font.h"
#include "u8g2_font.h"

void display_chinese_weather(weather_data_t *weather) {
    // 清屏
    lcd_fill_screen(RGB565(20, 30, 60));

    // 标题
    chinese_draw_string_centered(0, 10, LCD_WIDTH, 40, "北京天气", &chinese_font_16x16, RGB565(255, 255, 255), RGB565(20, 30, 60));

    // 分隔线
    lcd_draw_rect(10, 55, LCD_WIDTH - 20, 2, RGB565(100, 150, 255));

    // 城市和天气
    chinese_draw_string(20, 70, "北京", &chinese_font_16x16, RGB565(255, 255, 0), RGB565(20, 30, 60));
    chinese_draw_string(20, 100, weather->weather_text, &chinese_font_16x16, RGB565(200, 200, 200), RGB565(20, 30, 60));

    // 温度
    char temp_str[32];
    snprintf(temp_str, sizeof(temp_str), "%d°C", weather->temperature);
    u8g2_draw_string(20, 140, temp_str, &u8g2_font_10x20_tf, RGB565(255, 100, 100), RGB565(20, 30, 60));

    // 底部
    chinese_draw_string_centered(0, 280, LCD_WIDTH, 20, "实时天气", &chinese_font_16x16, RGB565(150, 150, 150), RGB565(20, 30, 60));
}
```

---

## 总结

| 方法 | 优点 | 缺点 | 适用场景 |
|-----|------|------|---------|
| Python工具 | 自动化、灵活 | 需要Python环境 | 推荐 |
| PCtoLCD2002 | 图形界面、简单 | 仅Windows | Windows用户 |
| 完整GB2312 | 字符全 | 占用空间大 | 需要大量汉字 |

**推荐方案**：使用Python工具生成指定字符的字库，既节省空间又灵活。