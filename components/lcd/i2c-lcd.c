/** Put this in the src folder **/

#include "i2c-lcd.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "unistd.h"

#define SLAVE_ADDRESS_LCD 0x27 // Change this according to your setup

esp_err_t err;
#define I2C_NUM I2C_NUM_0
static const char *TAG = "LCD";

// Function to send a command to the LCD
void lcd_send_cmd(char cmd) {
    uint8_t data_t[4];
    data_t[0] = (cmd & 0xF0) | 0x0C; // EN=1, RS=0
    data_t[1] = (cmd & 0xF0) | 0x08; // EN=0, RS=0
    data_t[2] = ((cmd << 4) & 0xF0) | 0x0C; // EN=1, RS=0
    data_t[3] = ((cmd << 4) & 0xF0) | 0x08; // EN=0, RS=0

    err = i2c_master_write_to_device(I2C_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000 / portTICK_PERIOD_MS);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "Error in sending command");
    }
}

// Function to send data to the LCD
void lcd_send_data(char data) {
    uint8_t data_t[4];
    data_t[0] = (data & 0xF0) | 0x0D; // EN=1, RS=1
    data_t[1] = (data & 0xF0) | 0x09; // EN=0, RS=1
    data_t[2] = ((data << 4) & 0xF0) | 0x0D; // EN=1, RS=1
    data_t[3] = ((data << 4) & 0xF0) | 0x09; // EN=0, RS=1

    err = i2c_master_write_to_device(I2C_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000 /portTICK_PERIOD_MS);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "Error in sending data");
    }
}

// Function to clear the LCD
void lcd_clear(void) {
    lcd_send_cmd(0x01);
    usleep(5000);
}

// Function to set cursor position
void lcd_put_cur(int row, int col) {
    switch (row) {
        case 0:
            col |= 0x80;  // Set cursor to row 0
            break;
        case 1:
            col |= 0xC0;  // Set cursor to row 1
            break;
    }
    lcd_send_cmd(col);
}

void lcd_init(void) {
    // 4-bit initialization
    usleep(50000);       // Wait for >40ms
    lcd_send_cmd(0x30);
    usleep(5000);        // Wait for >4.1ms
    lcd_send_cmd(0x30);
    usleep(100);         // Wait for >100us
    lcd_send_cmd(0x30);
    usleep(100);
    lcd_send_cmd(0x20);  // Set to 4-bit mode
    usleep(100);

    // Display initialization
    lcd_send_cmd(0x28);  // Function set: DL=0 (4-bit mode), N=1 (2-line display), F=0 (5x8 characters)
    lcd_send_cmd(0x08);  // Display off: D=0, C=0, B=0
    lcd_send_cmd(0x01);  // Clear display
    usleep(2000);        // Wait for command to complete
    lcd_send_cmd(0x06);  // Entry mode: I/D=1 (increment cursor), S=0 (no shift)
    lcd_send_cmd(0x0C);  // Display on: D=1 (display on), C=0 (cursor off), B=0 (blink off)
    usleep(100);
}

void lcd_send_string(char *str) {
    while (*str) {
        lcd_send_data(*str++);  // Send each character in the string
    }
}