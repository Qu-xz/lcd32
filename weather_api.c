#include "weather_api.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/altcp.h"
#include "lwip/altcp_tcp.h"
#include "lwip/dns.h"
#include "lwip/err.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// HTTP响应缓冲区
#define HTTP_BUFFER_SIZE 2048
static char http_response[HTTP_BUFFER_SIZE];
static int http_response_len = 0;

// 天气代码到英文的映射
static const char* weather_code_to_text(const char *code) {
    // 心知天气代码映射
    if (strcmp(code, "0") == 0) return "Sunny";
    if (strcmp(code, "1") == 0) return "Clear";
    if (strcmp(code, "2") == 0) return "Fair";
    if (strcmp(code, "3") == 0) return "Cloudy";
    if (strcmp(code, "4") == 0) return "Overcast";
    if (strcmp(code, "5") == 0) return "Shallow Fog";
    if (strcmp(code, "6") == 0) return "Moderate Fog";
    if (strcmp(code, "7") == 0) return "Dense Fog";
    if (strcmp(code, "8") == 0) return "Heavy Rain";
    if (strcmp(code, "9") == 0) return "Overcast";
    if (strcmp(code, "10") == 0) return "Light Rain";
    if (strcmp(code, "11") == 0) return "Moderate Rain";
    if (strcmp(code, "12") == 0) return "Heavy Rain";
    if (strcmp(code, "13") == 0) return "Storm";
    if (strcmp(code, "14") == 0) return "Heavy Storm";
    if (strcmp(code, "15") == 0) return "Severe Storm";
    if (strcmp(code, "16") == 0) return "Thunderstorm";
    if (strcmp(code, "17") == 0) return "Thundershower";
    if (strcmp(code, "18") == 0) return "Thundershower with Hail";
    if (strcmp(code, "19") == 0) return "Sleet";
    if (strcmp(code, "20") == 0) return "Light Snow";
    if (strcmp(code, "21") == 0) return "Moderate Snow";
    if (strcmp(code, "22") == 0) return "Heavy Snow";
    if (strcmp(code, "23") == 0) return "Blizzard";
    if (strcmp(code, "24") == 0) return "Fog";
    if (strcmp(code, "25") == 0) return "Haze";
    if (strcmp(code, "26") == 0) return "Haze";
    if (strcmp(code, "27") == 0) return "Haze";
    if (strcmp(code, "28") == 0) return "Sandstorm";
    if (strcmp(code, "29") == 0) return "Light to Moderate Rain";
    if (strcmp(code, "30") == 0) return "Moderate to Heavy Rain";
    if (strcmp(code, "31") == 0) return "Heavy Rain to Storm";
    if (strcmp(code, "32") == 0) return "Storm to Heavy Storm";
    if (strcmp(code, "33") == 0) return "Heavy Storm to Severe Storm";
    if (strcmp(code, "34") == 0) return "Small to Moderate Snow";
    if (strcmp(code, "35") == 0) return "Moderate to Heavy Snow";
    if (strcmp(code, "36") == 0) return "Heavy Snow to Blizzard";
    if (strcmp(code, "37") == 0) return "Dust";
    if (strcmp(code, "38") == 0) return "Sand";
    if (strcmp(code, "99") == 0) return "Unknown";
    return "Unknown";
}

// 简单的JSON解析函数
static void json_extract_string(const char *json, const char *key, char *output, int max_len) {
    char search_key[128];
    snprintf(search_key, sizeof(search_key), "\"%s\":\"", key);

    const char *start = strstr(json, search_key);
    if (start) {
        start += strlen(search_key);
        const char *end = strchr(start, '"');
        if (end) {
            int len = end - start;
            if (len > max_len - 1) len = max_len - 1;
            strncpy(output, start, len);
            output[len] = '\0';
        }
    }
}

static int json_extract_int(const char *json, const char *key) {
    char search_key[128];
    snprintf(search_key, sizeof(search_key), "\"%s\":", key);

    const char *start = strstr(json, search_key);
    if (start) {
        start += strlen(search_key);
        // 跳过可能的空格
        while (*start == ' ') start++;
        return atoi(start);
    }
    return 0;
}

// 提取嵌套对象的字符串值
static void json_extract_nested_string(const char *json, const char *parent, const char *key, char *output, int max_len) {
    char search_key[128];
    snprintf(search_key, sizeof(search_key), "\"%s\":{", parent);

    const char *parent_start = strstr(json, search_key);
    if (parent_start) {
        parent_start += strlen(search_key);

        // 在父对象中查找key
        char key_search[128];
        snprintf(key_search, sizeof(key_search), "\"%s\":\"", key);

        const char *start = strstr(parent_start, key_search);
        if (start) {
            start += strlen(key_search);
            const char *end = strchr(start, '"');
            if (end) {
                int len = end - start;
                if (len > max_len - 1) len = max_len - 1;
                strncpy(output, start, len);
                output[len] = '\0';
            }
        }
    }
}

// HTTP接收回调
static err_t http_recv_callback(void *arg, struct altcp_pcb *pcb, struct pbuf *p, err_t err) {
    if (!p) {
        // 连接关闭
        altcp_close(pcb);
        return ERR_OK;
    }

    // 复制数据到缓冲区
    if (http_response_len + p->len < HTTP_BUFFER_SIZE) {
        memcpy(http_response + http_response_len, p->payload, p->len);
        http_response_len += p->len;
    }

    pbuf_free(p);
    return ERR_OK;
}

// DNS解析回调
static void dns_lookup_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    struct altcp_pcb **pcb_ptr = (struct altcp_pcb **)callback_arg;

    if (ipaddr) {
        // 连接到服务器
        err_t err = altcp_connect(*pcb_ptr, ipaddr, 80, NULL);
        if (err != ERR_OK) {
            printf("Failed to connect to server\n");
            altcp_close(*pcb_ptr);
            *pcb_ptr = NULL;
        }
    } else {
        printf("DNS lookup failed\n");
        altcp_close(*pcb_ptr);
        *pcb_ptr = NULL;
    }
}

// 发送HTTP GET请求
static bool http_get_request(const char *host, const char *path, char *response, int *response_len) {
    struct altcp_pcb *pcb = altcp_tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!pcb) {
        printf("Failed to create TCP PCB\n");
        return false;
    }

    // 设置接收回调
    altcp_recv(pcb, http_recv_callback);
    altcp_poll(pcb, NULL, 1);

    // 清空响应缓冲区
    http_response_len = 0;
    memset(http_response, 0, HTTP_BUFFER_SIZE);

    // DNS解析
    ip_addr_t server_ip;
    err_t err = dns_gethostbyname(host, &server_ip, dns_lookup_callback, &pcb);
    if (err == ERR_OK) {
        // DNS缓存命中，直接连接
        err = altcp_connect(pcb, &server_ip, 80, NULL);
        if (err != ERR_OK) {
            printf("Failed to connect to server\n");
            altcp_close(pcb);
            return false;
        }
    } else if (err != ERR_INPROGRESS) {
        printf("DNS lookup failed\n");
        altcp_close(pcb);
        return false;
    }

    // 等待连接建立
    sleep_ms(1000);

    if (!pcb) {
        printf("Connection failed\n");
        return false;
    }

    // 构建HTTP请求
    char request[512];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: Pico-W-Weather/1.0\r\n"
             "Accept: */*\r\n"
             "Accept-Encoding: \r\n"
             "Connection: close\r\n"
             "\r\n",
             path, host);

    // 发送请求
    err = altcp_write(pcb, request, strlen(request), 0);
    if (err != ERR_OK) {
        printf("Failed to send request\n");
        altcp_close(pcb);
        return false;
    }

    altcp_output(pcb);

    // 等待响应
    sleep_ms(3000);

    // 复制响应
    if (http_response_len > 0) {
        memcpy(response, http_response, http_response_len);
        *response_len = http_response_len;
    }

    altcp_close(pcb);
    return http_response_len > 0;
}

// 初始化天气API
bool weather_init(void) {
    printf("Weather API initialized\n");
    return true;
}

// 获取天气数据
bool weather_get_data(const char *city, weather_data_t *data) {
    char url[256];
    snprintf(url, sizeof(url),
             "%s?key=%s&location=%s&language=zh-Hans&unit=c",
             WEATHER_API_PATH, WEATHER_API_KEY, city);

    printf("Requesting weather data: %s\n", url);

    char response[HTTP_BUFFER_SIZE];
    int response_len = 0;

    if (!http_get_request(WEATHER_API_HOST, url, response, &response_len)) {
        printf("Failed to get weather data\n");
        return false;
    }

    printf("Response received (%d bytes)\n", response_len);

    // 查找JSON数据部分（跳过HTTP头）
    char *json_start = strstr(response, "\r\n\r\n");
    if (!json_start) {
        printf("Invalid response format\n");
        return false;
    }

    // 打印HTTP头部分
    int header_len = json_start - response;
    printf("HTTP Header (%d bytes):\n", header_len);
    for (int i = 0; i < header_len && i < 200; i++) {
        printf("%c", response[i]);
    }
    printf("\n");

    json_start += 4;

    printf("JSON data: %s\n", json_start);

    // 解析JSON数据
    memset(data, 0, sizeof(weather_data_t));

    // 提取城市名称（从location对象中提取name）
    json_extract_nested_string(json_start, "location", "name", data->city, sizeof(data->city));

    // 提取天气描述（中文）
    json_extract_string(json_start, "text", data->weather_text, sizeof(data->weather_text));

    // 提取天气代码
    json_extract_string(json_start, "code", data->code, sizeof(data->code));

    // 根据代码获取英文天气描述
    const char *weather_en = weather_code_to_text(data->code);
    strncpy(data->weather_text_en, weather_en, sizeof(data->weather_text_en) - 1);

    // 提取温度（字符串转整数）
    char temp_str[16];
    json_extract_string(json_start, "temperature", temp_str, sizeof(temp_str));
    data->temperature = atoi(temp_str);

    // 提取最后更新时间
    json_extract_string(json_start, "last_update", data->last_update, sizeof(data->last_update));

    data->updated = true;

    printf("Weather: %s, %s (%s), %d°C, code: %s, update: %s\n",
           data->city, data->weather_text, data->weather_text_en, data->temperature, data->code, data->last_update);

    return true;
}

// 更新北京天气
bool weather_update_beijing(weather_data_t *data) {
    return weather_get_data("beijing", data);
}