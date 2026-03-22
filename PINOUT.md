# ILI9341 3.2寸LCD 8位并口连接说明

## 文件结构

```
lcd32/
├── lcd32.h           # LCD驱动头文件（包含引脚定义和函数声明）
├── lcd32_driver.c    # LCD驱动实现文件
├── main.c            # 主程序文件（测试代码）
├── CMakeLists.txt    # CMake构建配置
└── PINOUT.md         # 本文件
```

## 硬件连接

### 数据引脚 (D0-D7)
| LCD引脚 | Pico W引脚 | 说明 |
|---------|-----------|------|
| D0      | GP15      | 数据位0 |
| D1      | GP14      | 数据位1 |
| D2      | GP2       | 数据位2 |
| D3      | GP3       | 数据位3 |
| D4      | GP4       | 数据位4 |
| D5      | GP5       | 数据位5 |
| D6      | GP6       | 数据位6 |
| D7      | GP7       | 数据位7 |

### 控制引脚
| LCD引脚 | Pico W引脚 | 说明 |
|---------|-----------|------|
| RS      | GP8       | 命令/数据选择 (0=命令, 1=数据) |
| WR      | GP9       | 写信号 (低电平有效) |
| RD      | GP10      | 读信号 (低电平有效) |
| CS      | GP11      | 片选 (低电平有效) |
| RST     | GP12      | 复位 |
| BL      | GP13      | 背光 (高电平开启) |

### 电源引脚
| LCD引脚 | Pico W引脚 | 说明 |
|---------|-----------|------|
| VCC     | 3.3V      | 3.3V电源 |
| GND     | GND       | 地线 |

## 引脚定义说明

### 数据引脚 (GP14, GP15, GP2-GP7)
- 8位并行数据总线
- 用于传输8位数据或命令

### 控制引脚
- **RS (Register Select)**: 选择命令或数据模式
  - 低电平: 命令模式
  - 高电平: 数据模式

- **WR (Write)**: 写信号
  - 低电平有效
  - 下降沿触发数据写入

- **RD (Read)**: 读信号
  - 低电平有效
  - 本程序中未使用读功能

- **CS (Chip Select)**: 片选
  - 低电平有效
  - 选择LCD芯片

- **RST (Reset)**: 复位
  - 低电平复位
  - 上电时需要复位

- **BL (Backlight)**: 背光控制
  - 高电平: 开启背光
  - 低电平: 关闭背光

## 编译和上传

### 编译
```bash
cd build
cmake --build .
```

### 上传到Pico W
```bash
# 按住BOOTSEL按钮后插入USB
cp lcd32.uf2 /Volumes/RPI-RP2/
```

## 测试程序功能

程序启动后会执行以下测试:
1. 初始化LCD屏幕
2. 填充黑色背景
3. 绘制红色矩形
4. 绘制绿色矩形
5. 绘制蓝色矩形
6. 绘制黄色矩形

## 修改引脚定义

如果需要修改引脚定义，请编辑 `lcd32.h` 文件中的宏定义:

```c
// 数据引脚 D0-D7
#define LCD_D0  15
#define LCD_D1  14
#define LCD_D2  2
#define LCD_D3  3
#define LCD_D4  4
#define LCD_D5  5
#define LCD_D6  6
#define LCD_D7  7

// 控制引脚
#define LCD_RS  8
#define LCD_WR  9
#define LCD_RD  10
#define LCD_CS  11
#define LCD_RST 12
#define LCD_BL  13  // 背光
```

## 注意事项

1. **电压**: LCD使用3.3V供电，不要连接5V
2. **背光**: 背光可以连接到3.3V或通过PWM控制亮度
3. **时序**: 程序中已经设置了适当的延时，确保信号稳定
4. **引脚冲突**: 确保使用的GPIO引脚没有与其他外设冲突

## 扩展功能

当前程序提供了基本功能:
- `lcd_fill_screen(color)` - 填充屏幕
- `lcd_draw_pixel(x, y, color)` - 绘制像素
- `lcd_draw_rect(x, y, w, h, color)` - 绘制矩形
- `lcd_set_window(x0, y0, x1, y1)` - 设置显示窗口

可以基于这些函数开发更复杂的图形功能。