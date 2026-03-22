#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pico/stdlib.h"
#include "lcd32.h"
#include "u8g2_font.h"
#include "chinese_font.h"
#include "wifi_config.h"
#include "weather_api.h"

// 从 last_update 字符串提取日期 (格式: 2024-03-14T12:00:00+08:00)
static void extract_date_from_update(const char *last_update, int *year, int *month, int *day)
{
    if (last_update && strlen(last_update) >= 10)
    {
        // 解析 YYYY-MM-DD
        *year = atoi(last_update);
        const char *month_str = strchr(last_update, '-');
        if (month_str)
        {
            month_str++;
            *month = atoi(month_str);
            const char *day_str = strchr(month_str, '-');
            if (day_str)
            {
                day_str++;
                *day = atoi(day_str);
                return;
            }
        }
    }
    // 默认值
    *year = 2024;
    *month = 1;
    *day = 1;
}

// 显示天气信息
void display_weather(weather_data_t *weather)
{
    // 从天气API的更新时间提取日期
    int year, month, day;
    extract_date_from_update(weather->last_update, &year, &month, &day);

    // 创建日期字符串
    char date_str[32];
    snprintf(date_str, sizeof(date_str), "%04d年%02d月%02d日", year, month, day);

    // 清屏 - 深蓝色背景
    lcd_fill_screen(RGB565(20, 30, 60));

    // 标题 - 显示日期
    chinese_draw_string_centered(0, 10, LCD_WIDTH, 40, date_str, &chinese_font_16x16, RGB565(255, 255, 255), RGB565(20, 30, 60));

    // 分隔线
    lcd_draw_rect(10, 55, LCD_WIDTH - 20, 2, RGB565(100, 150, 255));

    // 城市名称 - 使用中文字体
    chinese_draw_string_centered(0, 70, LCD_WIDTH, 30, "北京", &chinese_font_16x16, RGB565(255, 255, 0), RGB565(20, 30, 60));

    // 天气描述 - 使用中文字体
    chinese_draw_string_centered(0, 110, LCD_WIDTH, 30, weather->weather_text, &chinese_font_16x16, RGB565(200, 200, 200), RGB565(20, 30, 60));

    // 温度显示 - 使用20x40大字体
    char temp_str[16];
    snprintf(temp_str, sizeof(temp_str), "%d", weather->temperature);

    // 计算温度字符串宽度（每个数字20像素）
    int temp_len = strlen(temp_str);
    int num_width = temp_len * 20;

    // 数字居中显示（只考虑数字）
    uint16_t start_x = (LCD_WIDTH - num_width) / 2;
    uint16_t pos_x = start_x;

    // 绘制温度数字
    for (int i = 0; i < temp_len; i++)
    {
        u8g2_draw_char(pos_x, 160, temp_str[i], &u8g2_font_20x40_nums, RGB565(255, 100, 100), RGB565(20, 30, 60));
        pos_x += 20;
    }

    // 绘制°符号在数字之后，往上偏移
    u8g2_draw_char(start_x + num_width, 155, 176, &u8g2_font_20x40_nums, RGB565(255, 100, 100), RGB565(20, 30, 60));

    // 底部信息 - 使用中文字体
    chinese_draw_string_centered(0, 280, LCD_WIDTH, 20, "实时天气", &chinese_font_16x16, RGB565(150, 150, 150), RGB565(20, 30, 60));
}

int main()
{
    stdio_init_all();

    // 初始化随机数种子
    srand(time(NULL));

    // 初始化WiFi配置
    wifi_config_init();

    // 连接到固定WiFi网络
    printf("Connecting to WiFi...\n");
    if (!wifi_connect_with_fixed_config())
    {
        printf("Failed to connect to WiFi\n");
        return -1;
    }

    // 初始化天气API
    weather_init();

    // 初始化LCD
    lcd_init();
    printf("LCD initialized\n");

    // 设置背光亮度为50%
    lcd_backlight_set_brightness(50);
    printf("Backlight set to 50%%\n");

    // 显示加载中
    lcd_fill_screen(RGB565(20, 30, 60));
    u8g2_draw_string_centered(0, 140, LCD_WIDTH, 40, "Loading...", &u8g2_font_10x20_tf, RGB565(255, 255, 255), RGB565(20, 30, 60));

    // 获取天气数据
    weather_data_t weather;
    printf("Fetching weather data...\n");

    if (weather_update_beijing(&weather))
    {
        printf("Weather data received\n");
        display_weather(&weather);
    }
    else
    {
        printf("Failed to get weather data\n");
        // 显示错误信息
        lcd_fill_screen(RGB565(60, 20, 20));
        u8g2_draw_string_centered(0, 120, LCD_WIDTH, 30, "Weather Error", &u8g2_font_10x20_tf, RGB565(255, 255, 255), RGB565(60, 20, 20));
        u8g2_draw_string_centered(0, 160, LCD_WIDTH, 30, "Check Network", &u8g2_font_8x13_tf, RGB565(255, 255, 255), RGB565(60, 20, 20));
    }

    printf("Weather display started\n");

    // 每5分钟更新一次天气
    while (true)
    {
        sleep_ms(300000); // 5分钟

        printf("Updating weather...\n");
        if (weather_update_beijing(&weather))
        {
            display_weather(&weather);
            printf("Weather updated\n");
        }
        else
        {
            printf("Failed to update weather\n");
        }
    }
}