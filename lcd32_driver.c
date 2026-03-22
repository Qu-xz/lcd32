#include "lcd32.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

// 初始化数据引脚
static void lcd_init_data_pins(void) {
    const uint8_t data_pins[] = {LCD_D0, LCD_D1, LCD_D2, LCD_D3, LCD_D4, LCD_D5, LCD_D6, LCD_D7};
    for (int i = 0; i < 8; i++) {
        gpio_init(data_pins[i]);
        gpio_set_dir(data_pins[i], GPIO_OUT);
    }
}

// 初始化控制引脚
static void lcd_init_control_pins(void) {
    gpio_init(LCD_RS);
    gpio_init(LCD_WR);
    gpio_init(LCD_RD);
    gpio_init(LCD_CS);
    gpio_init(LCD_RST);

    gpio_set_dir(LCD_RS, GPIO_OUT);
    gpio_set_dir(LCD_WR, GPIO_OUT);
    gpio_set_dir(LCD_RD, GPIO_OUT);
    gpio_set_dir(LCD_CS, GPIO_OUT);
    gpio_set_dir(LCD_RST, GPIO_OUT);

    // 初始状态
    gpio_put(LCD_CS, 1);   // 片选无效
    gpio_put(LCD_WR, 1);   // 写信号无效
    gpio_put(LCD_RD, 1);   // 读信号无效
    gpio_put(LCD_RST, 1);  // 复位无效
}

// 写8位数据到数据总线
static void lcd_write_data_bus(uint8_t data) {
    gpio_put(LCD_D0, (data >> 0) & 0x01);
    gpio_put(LCD_D1, (data >> 1) & 0x01);
    gpio_put(LCD_D2, (data >> 2) & 0x01);
    gpio_put(LCD_D3, (data >> 3) & 0x01);
    gpio_put(LCD_D4, (data >> 4) & 0x01);
    gpio_put(LCD_D5, (data >> 5) & 0x01);
    gpio_put(LCD_D6, (data >> 6) & 0x01);
    gpio_put(LCD_D7, (data >> 7) & 0x01);
}

// 写命令
static void lcd_write_command(uint8_t cmd) {
    gpio_put(LCD_CS, 0);   // 片选有效
    gpio_put(LCD_RS, 0);   // 命令模式

    lcd_write_data_bus(cmd);

    gpio_put(LCD_WR, 0);   // 写信号有效
    sleep_us(1);
    gpio_put(LCD_WR, 1);   // 写信号无效

    gpio_put(LCD_CS, 1);   // 片选无效
}

// 写数据
static void lcd_write_data(uint8_t data) {
    gpio_put(LCD_CS, 0);   // 片选有效
    gpio_put(LCD_RS, 1);   // 数据模式

    lcd_write_data_bus(data);

    gpio_put(LCD_WR, 0);   // 写信号有效
    sleep_us(1);
    gpio_put(LCD_WR, 1);   // 写信号无效

    gpio_put(LCD_CS, 1);   // 片选无效
}

// 硬件复位
static void lcd_hard_reset(void) {
    gpio_put(LCD_RST, 0);
    sleep_ms(10);
    gpio_put(LCD_RST, 1);
    sleep_ms(120);
}

// 初始化ILI9341
void lcd_init(void) {
    lcd_init_data_pins();
    lcd_init_control_pins();
    lcd_backlight_init();  // 初始化背光PWM

    // 硬件复位
    lcd_hard_reset();

    // 软件复位
    lcd_write_command(ILI9341_SWRESET);
    sleep_ms(120);

    // 退出睡眠模式
    lcd_write_command(ILI9341_SLPOUT);
    sleep_ms(120);

    // 像素格式设置 - RGB565
    lcd_write_command(ILI9341_PIXFMT);
    lcd_write_data(0x55);
    sleep_ms(10);

    // 内存访问控制
    lcd_write_command(ILI9341_MADCTL);
    lcd_write_data(0x48);  // BGR模式 + MX (水平翻转修正)

    // 帧率控制
    lcd_write_command(ILI9341_FRMCTR1);
    lcd_write_data(0x00);
    lcd_write_data(0x18);

    // 显示功能控制
    lcd_write_command(ILI9341_DFUNCTR);
    lcd_write_data(0x08);
    lcd_write_data(0x82);
    lcd_write_data(0x27);

    // 电源控制
    lcd_write_command(ILI9341_PWCTR1);
    lcd_write_data(0x23);

    lcd_write_command(ILI9341_PWCTR2);
    lcd_write_data(0x10);

    lcd_write_command(ILI9341_VMCTR1);
    lcd_write_data(0x3e);
    lcd_write_data(0x28);

    lcd_write_command(ILI9341_VMCTR2);
    lcd_write_data(0x86);

    // 电源控制序列
    lcd_write_command(ILI9341_PWCTR3);
    lcd_write_data(0x00);
    lcd_write_data(0x00);

    lcd_write_command(ILI9341_PWCTR4);
    lcd_write_data(0x00);
    lcd_write_data(0x00);

    lcd_write_command(ILI9341_PWCTR5);
    lcd_write_data(0x00);
    lcd_write_data(0x00);

    // Gamma设置
    lcd_write_command(ILI9341_GMCTRP1);
    lcd_write_data(0x0f);
    lcd_write_data(0x31);
    lcd_write_data(0x2b);
    lcd_write_data(0x0c);
    lcd_write_data(0x0e);
    lcd_write_data(0x08);
    lcd_write_data(0x4e);
    lcd_write_data(0xf1);
    lcd_write_data(0x37);
    lcd_write_data(0x07);
    lcd_write_data(0x10);
    lcd_write_data(0x03);
    lcd_write_data(0x0e);
    lcd_write_data(0x09);
    lcd_write_data(0x00);

    lcd_write_command(ILI9341_GMCTRN1);
    lcd_write_data(0x00);
    lcd_write_data(0x0e);
    lcd_write_data(0x14);
    lcd_write_data(0x03);
    lcd_write_data(0x11);
    lcd_write_data(0x07);
    lcd_write_data(0x31);
    lcd_write_data(0xc1);
    lcd_write_data(0x48);
    lcd_write_data(0x08);
    lcd_write_data(0x0f);
    lcd_write_data(0x0c);
    lcd_write_data(0x31);
    lcd_write_data(0x36);
    lcd_write_data(0x0f);

    // 开启显示
    lcd_write_command(ILI9341_SLPOUT);
    sleep_ms(120);

    lcd_write_command(ILI9341_DISPON);
    sleep_ms(100);

    // 清屏 - 黑色
    lcd_fill_screen(0x0000);
}

// 设置显示窗口
void lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    lcd_write_command(ILI9341_CASET);
    lcd_write_data(x0 >> 8);
    lcd_write_data(x0 & 0xFF);
    lcd_write_data(x1 >> 8);
    lcd_write_data(x1 & 0xFF);

    lcd_write_command(ILI9341_PASET);
    lcd_write_data(y0 >> 8);
    lcd_write_data(y0 & 0xFF);
    lcd_write_data(y1 >> 8);
    lcd_write_data(y1 & 0xFF);

    lcd_write_command(ILI9341_RAMWR);
}

// 填充屏幕
void lcd_fill_screen(uint16_t color) {
    lcd_set_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
        lcd_write_data(color >> 8);
        lcd_write_data(color & 0xFF);
    }
}

// 绘制像素
void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    lcd_set_window(x, y, x, y);
    lcd_write_data(color >> 8);
    lcd_write_data(color & 0xFF);
}

// 绘制矩形
void lcd_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    lcd_set_window(x, y, x + w - 1, y + h - 1);
    for (int i = 0; i < w * h; i++) {
        lcd_write_data(color >> 8);
        lcd_write_data(color & 0xFF);
    }
}

// 开启背光
void lcd_backlight_on(void) {
    lcd_backlight_set_brightness(100);
}

// 关闭背光
void lcd_backlight_off(void) {
    lcd_backlight_set_brightness(0);
}

// 初始化背光PWM
void lcd_backlight_init(void) {
    // 设置GPIO为PWM功能
    gpio_set_function(LCD_BL, GPIO_FUNC_PWM);
    
    // 获取PWM切片号
    uint slice_num = pwm_gpio_to_slice_num(LCD_BL);
    
    // 配置PWM
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 1.0f);  // 时钟分频
    pwm_config_set_wrap(&config, 255);     // 8位分辨率 (0-255)
    
    // 初始化PWM
    pwm_init(slice_num, &config, true);
    
    // 初始亮度为0
    pwm_set_gpio_level(LCD_BL, 0);
}

// 设置背光亮度 (0-100)
void lcd_backlight_set_brightness(uint8_t brightness) {
    // 限制范围
    if (brightness > 100) {
        brightness = 100;
    }
    
    // 将0-100映射到0-255
    uint16_t level = (brightness * 255) / 100;
    
    // 设置PWM占空比
    pwm_set_gpio_level(LCD_BL, level);
}