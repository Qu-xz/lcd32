#include "wifi_config.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <string.h>
#include <stdio.h>

// 固定WiFi配置 - 在这里修改你的WiFi名称和密码
#define WIFI_SSID "yoyoyo"
#define WIFI_PASSWORD "yoyoyo55"

// 全局WiFi配置
static wifi_config_t g_wifi_config = {0};
static bool g_wifi_connected = false;

// 初始化WiFi配置
void wifi_config_init(void)
{
    memset(&g_wifi_config, 0, sizeof(wifi_config_t));
    g_wifi_config.configured = false;
    g_wifi_connected = false;
}

// 连接到WiFi网络
bool wifi_connect_to_network(const char *ssid, const char *password)
{
    printf("Connecting to WiFi: %s\n", ssid);

    // 初始化WiFi芯片
    if (cyw43_arch_init())
    {
        printf("WiFi init failed\n");
        return false;
    }

    // 启用Station模式
    cyw43_arch_enable_sta_mode();

    // 连接到WiFi
    if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("Failed to connect to WiFi\n");
        g_wifi_connected = false;
        return false;
    }

    printf("Connected to WiFi!\n");
    g_wifi_connected = true;

    // 保存配置
    strncpy(g_wifi_config.ssid, ssid, WIFI_SSID_MAX_LEN);
    strncpy(g_wifi_config.password, password, WIFI_PASSWORD_MAX_LEN);
    g_wifi_config.configured = true;

    // 打印IP地址
    uint8_t *ip_address = (uint8_t *)&(cyw43_state.netif[0].ip_addr.addr);
    printf("IP address: %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);

    return true;
}

// 使用固定WiFi配置连接
bool wifi_connect_with_fixed_config(void)
{
    return wifi_connect_to_network(WIFI_SSID, WIFI_PASSWORD);
}

// 检查是否已连接
bool wifi_is_connected(void)
{
    return g_wifi_connected;
}

// 启动AP模式（已禁用）
bool wifi_start_ap_mode(void)
{
    printf("AP mode disabled. Using fixed WiFi configuration.\n");
    return false;
}

// 启动HTTP服务器（已禁用）
void wifi_http_server_start(void)
{
    printf("HTTP server disabled. Using fixed WiFi configuration.\n");
}

// 停止HTTP服务器（已禁用）
void wifi_http_server_stop(void)
{
    printf("HTTP server disabled.\n");
}