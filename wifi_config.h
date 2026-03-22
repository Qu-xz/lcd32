#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#define WIFI_SSID_MAX_LEN 32
#define WIFI_PASSWORD_MAX_LEN 64

// WiFi配置结构体
typedef struct {
    char ssid[WIFI_SSID_MAX_LEN + 1];
    char password[WIFI_PASSWORD_MAX_LEN + 1];
    bool configured;
} wifi_config_t;

// 函数声明
void wifi_config_init(void);
bool wifi_connect_to_network(const char *ssid, const char *password);
bool wifi_connect_with_fixed_config(void);
bool wifi_is_connected(void);
bool wifi_start_ap_mode(void);
void wifi_http_server_start(void);
void wifi_http_server_stop(void);

#endif // WIFI_CONFIG_H