#ifndef I2C_LCD_H
#define I2C_LCD_H

#include "esp_err.h"
#include "driver/i2c.h"

// Macro định nghĩa địa chỉ I2C của LCD
#define SLAVE_ADDRESS_LCD 0x27 // Thay đổi theo thiết lập của bạn

// Định nghĩa I2C port được sử dụng
#define I2C_NUM I2C_NUM_0

// Hàm khởi tạo LCD
void lcd_init(void);

// Hàm gửi lệnh tới LCD
void lcd_send_cmd(char cmd);

// Hàm gửi dữ liệu tới LCD
void lcd_send_data(char data);

// Hàm xóa màn hình LCD
void lcd_clear(void);

// Hàm đặt vị trí con trỏ
void lcd_put_cur(int row, int col);

// Hàm gửi chuỗi ký tự tới LCD
void lcd_send_string(char *str);

#endif // I2C_LCD_H