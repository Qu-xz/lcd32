#ifndef U8G2_FONT_H
#define U8G2_FONT_H

#include <stdint.h>
#include <stdbool.h>
#include "lcd32.h"

// u8g2字体结构
typedef struct {
    const uint8_t *glyph_data;  // 字形数据
    uint8_t width;              // 字体宽度
    uint8_t height;             // 字体高度
    uint8_t start_char;         // 起始字符
    uint8_t end_char;           // 结束字符
} u8g2_font_t;

// 可用字体
extern const u8g2_font_t u8g2_font_6x10_tf;
extern const u8g2_font_t u8g2_font_8x13_tf;
extern const u8g2_font_t u8g2_font_10x20_tf;
extern const u8g2_font_t u8g2_font_20x40_nums;  // 新增：大数字字体
extern const u8g2_font_t u8g2_font_ncenB14_tr;

// 函数声明
void u8g2_draw_char(uint16_t x, uint16_t y, int c, const u8g2_font_t *font, uint16_t color, uint16_t bg_color);
void u8g2_draw_string(uint16_t x, uint16_t y, const char *str, const u8g2_font_t *font, uint16_t color, uint16_t bg_color);
void u8g2_draw_string_centered(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *str, const u8g2_font_t *font, uint16_t color, uint16_t bg_color);
uint16_t u8g2_get_string_width(const char *str, const u8g2_font_t *font);
void u8g2_draw_temp_string(uint16_t x, uint16_t y, const char *temp_str, uint16_t color, uint16_t bg_color);  // 新增：温度显示函数

#endif // U8G2_FONT_H