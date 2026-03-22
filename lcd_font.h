#ifndef LCD_FONT_H
#define LCD_FONT_H

#include <stdint.h>
#include "lcd32.h"

// 8x16 ASCII字体
extern const uint8_t font_8x16[][16];

// 文本显示函数
void lcd_draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg_color);
void lcd_draw_string(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg_color);
void lcd_draw_string_centered(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *str, uint16_t color, uint16_t bg_color);

#endif // LCD_FONT_H