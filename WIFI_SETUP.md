# WiFi固定配置使用说明

## 功能概述

程序使用固定WiFi配置，启动时自动连接到预设的WiFi网络。

## 配置WiFi

### 修改WiFi名称和密码

编辑 `wifi_config.c` 文件，修改以下宏定义:

```c
#define WIFI_SSID     "Your_WiFi_SSID"
#define WIFI_PASSWORD "Your_WiFi_Password"
```

将 `Your_WiFi_SSID` 替换为你的WiFi名称，将 `Your_WiFi_Password` 替换为你的WiFi密码。

### 示例

```c
#define WIFI_SSID     "MyHomeWiFi"
#define WIFI_PASSWORD "mypassword123"
```

## 使用步骤

### 1. 配置WiFi

编辑 `wifi_config.c` 文件，设置你的WiFi名称和密码。

### 2. 编译程序

```bash
cd build
cmake --build .
```

### 3. 上传程序到Pico W

```bash
# 按住BOOTSEL按钮后插入USB
cp build/lcd32.uf2 /Volumes/RPI-RP2/
```

### 4. 查看连接状态

通过串口监视器查看连接状态和IP地址。

## 串口输出信息

程序运行时会在串口输出以下信息:

```
Connecting to WiFi: Your_WiFi_SSID
Connected to WiFi!
IP address: 192.168.1.100
LCD initialized
Backlight on
Starting color animation...
Colors updated: 0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1357, 0x2468
...
```

如果连接失败:

```
Connecting to WiFi: Your_WiFi_SSID
Failed to connect to WiFi
```

## 注意事项

1. **WiFi配置**: 必须在编译前修改 `wifi_config.c` 文件中的WiFi配置
2. **连接超时**: WiFi连接超时时间为30秒
3. **密码安全**: WiFi密码以明文形式存储在代码中，请注意安全
4. **串口监视**: 建议通过串口监视器查看连接状态和调试信息
5. **网络要求**: WiFi网络必须支持WPA2-AES加密

## 文件结构

```
lcd32/
├── wifi_config.h      # WiFi配置头文件
├── wifi_config.c      # WiFi配置实现（固定WiFi配置）
├── main.c             # 主程序（集成WiFi功能）
├── lcd32.h            # LCD驱动头文件
├── lcd32_driver.c     # LCD驱动实现
└── CMakeLists.txt     # 构建配置
```

## 故障排除

### WiFi连接失败
- 检查WiFi名称和密码是否正确
- 确认WiFi网络是否可访问
- 确认WiFi网络支持WPA2-AES加密
- 查看串口输出的错误信息

### 无法获取IP地址
- 检查路由器DHCP功能是否正常
- 确认WiFi网络连接正常
- 尝试重启路由器和Pico W

### 编译错误
- 确认已正确安装Pico SDK
- 检查CMakeLists.txt配置是否正确
- 确认WiFi相关库已正确链接

## 扩展功能

可以基于现有代码扩展以下功能:
- 从Flash读取WiFi配置
- 添加WiFi重连机制
- 实现WiFi状态查询功能
- 添加网络通信功能（如HTTP客户端、MQTT等）

## API函数说明

### wifi_config_init()
初始化WiFi配置结构体。

### wifi_connect_with_fixed_config()
使用预设的WiFi配置连接到WiFi网络。

### wifi_connect_to_network(ssid, password)
使用指定的SSID和密码连接到WiFi网络。

### wifi_is_connected()
检查WiFi是否已连接。

返回值:
- `true`: 已连接
- `false`: 未连接

## 示例代码

### 基本使用

```c
#include "wifi_config.h"

int main() {
    stdio_init_all();

    // 初始化WiFi
    wifi_config_init();

    // 连接到WiFi
    if (wifi_connect_with_fixed_config()) {
        printf("WiFi connected!\n");
    } else {
        printf("WiFi connection failed!\n");
    }

    // 检查连接状态
    if (wifi_is_connected()) {
        printf("WiFi is connected\n");
    }

    while (true) {
        // 主循环
        sleep_ms(1000);
    }
}
```

### 自定义SSID和密码

```c
#include "wifi_config.h"

int main() {
    stdio_init_all();

    wifi_config_init();

    // 使用自定义SSID和密码连接
    if (wifi_connect_to_network("MyWiFi", "MyPassword")) {
        printf("Connected to MyWiFi\n");
    }

    while (true) {
        sleep_ms(1000);
    }
}
```