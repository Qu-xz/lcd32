#ifndef CHINESE_FONT_H
#define CHINESE_FONT_H

#include <stdint.h>
#include <stdbool.h>
#include "lcd32.h"

// 中文字体结构
typedef struct {
    const uint8_t *glyph_data;  // 字形数据
    uint16_t width;              // 字体宽度
    uint16_t height;             // 字体高度
    uint16_t char_count;         // 字符数量
    const char *char_map;        // 字符映射表
} chinese_font_t;

// 可用中文字体
extern const chinese_font_t chinese_font_16x16;
extern const chinese_font_t chinese_font_24x24;

// 函数声明
int chinese_find_char_index(const char *str, const chinese_font_t *font);
void chinese_draw_char(uint16_t x, uint16_t y, const char *str, const chinese_font_t *font, uint16_t color, uint16_t bg_color);
void chinese_draw_string(uint16_t x, uint16_t y, const char *str, const chinese_font_t *font, uint16_t color, uint16_t bg_color);
void chinese_draw_string_centered(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *str, const chinese_font_t *font, uint16_t color, uint16_t bg_color);

#endif // CHINESE_FONT_H