#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <stdint.h>
#include <stdbool.h>

// 心知天气API配置
#define WEATHER_API_KEY "SVQx2s0JmTLDLFbzp" // 在这里替换为你的API Key
#define WEATHER_API_HOST "api.seniverse.com"
#define WEATHER_API_PATH "/v3/weather/now.json"

// 天气数据结构
typedef struct
{
    char city[32];         // 城市名称
    char weather_text[32]; // 天气描述
    char weather_text_en[32]; // 英文天气描述
    int temperature;       // 温度（摄氏度）
    char code[8];          // 天气代码
    char last_update[32];  // 最后更新时间 (格式: 2024-03-14T12:00:00+08:00)
    bool updated;          // 是否已更新
} weather_data_t;

// 函数声明
bool weather_init(void);
bool weather_get_data(const char *city, weather_data_t *data);
bool weather_update_beijing(weather_data_t *data);

#endif // WEATHER_API_H