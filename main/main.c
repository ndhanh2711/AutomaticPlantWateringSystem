#include <stdio.h>
#include <unistd.h>      // for usleep
#include "driver/i2c.h"  // for I2C communication
#include "esp_log.h"     // for ESP_LOGI
#include "i2c-lcd.h"
#include "driver/ledc.h"
#include "esp_err.h"


#define TAG "LCD"                  // Logging tag
#define I2C_MASTER_NUM I2C_NUM_0   // Define I2C port
#define SLAVE_ADDRESS_LCD 0x27     // I2C address of the LCD (modify if needed)


#define LEDC_TIMER          LEDC_TIMER_0
#define LEDC_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO      (4) // GPIO4
#define LEDC_CHANNEL        LEDC_CHANNEL_0
#define LEDC_FREQUENCY      (5000) // Frequency in Hz

// Send a command to the LCD
static esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };

    i2c_param_config(I2C_NUM_0, &conf);

    return i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
} 
void lcd_send_cmd(char cmd) {
    esp_err_t err;
    uint8_t data_u, data_l;    // Sửa từ `char` thành `uint8_t`
    uint8_t data_t[4];         // Sửa từ `char` thành `uint8_t`

    data_u = (cmd & 0xF0);
    data_l = ((cmd << 4) & 0xF0);

    data_t[0] = data_u | 0x0C;  // EN=1, RS=0
    data_t[1] = data_u | 0x08;  // EN=0, RS=0
    data_t[2] = data_l | 0x0C;  // EN=1, RS=0
    data_t[3] = data_l | 0x08;  // EN=0, RS=0

    err = i2c_master_write_to_device(I2C_MASTER_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "Error %d while sending command to LCD", err);
    }
}

void lcd_send_data(char data) {
    esp_err_t err;
    uint8_t data_u, data_l;    // Sửa từ `char` thành `uint8_t`
    uint8_t data_t[4];         // Sửa từ `char` thành `uint8_t`

    data_u = (data & 0xF0);
    data_l = ((data << 4) & 0xF0);

    data_t[0] = data_u | 0x0D;  // EN=1, RS=1
    data_t[1] = data_u | 0x09;  // EN=0, RS=1
    data_t[2] = data_l | 0x0D;  // EN=1, RS=1
    data_t[3] = data_l | 0x09;  // EN=0, RS=1

    err = i2c_master_write_to_device(I2C_MASTER_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "Error %d while sending data to LCD", err);
    }
}


// Initialize the LCD in 4-bit mode
void lcd_init(void) {
    usleep(50000);        // Wait for >40ms after power-up
    lcd_send_cmd(0x30);
    usleep(4500);         
    lcd_send_cmd(0x30);
    usleep(200);          
    lcd_send_cmd(0x30);
    usleep(200);          
    lcd_send_cmd(0x20);   // 4-bit mode
    usleep(200);

    lcd_send_cmd(0x28);   // Function set: 4-bit mode, 2 lines, 5x8 font
    usleep(1000);
    lcd_send_cmd(0x08);   // Display off
    usleep(1000);
    lcd_send_cmd(0x01);   // Clear display
    usleep(5000);         // Clear takes longer
    lcd_send_cmd(0x06);   // Entry mode set: Increment cursor, no shift
    usleep(1000);
    lcd_send_cmd(0x0C);   // Display on, cursor off, blink off
    usleep(2000);
}

// Clear the LCD screen
void lcd_clear(void) {
    lcd_send_cmd(0x01);  // Clear display command
    usleep(5000);        // Allow time for the command to execute
}

// Set cursor position
void lcd_put_cur(int row, int col) {
    switch (row) {
        case 0:
            col |= 0x80;  // Row 0 starts at 0x80
            break;
        case 1:
            col |= 0xC0;  // Row 1 starts at 0xC0
            break;
    }
    lcd_send_cmd(col);
}

// Send a string to the LCD
void lcd_send_string(char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}

// Hàm khởi tạo LEDC
void init_ledc(void) {
    // Cấu hình bộ định thời LEDC
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_TIMER_13_BIT, // Độ phân giải 13 bit
        .freq_hz          = LEDC_FREQUENCY,   // Tần số 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Cấu hình kênh LEDC
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = LEDC_OUTPUT_IO,
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER,
        .duty           = 0, // Duty cycle ban đầu
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

// Hàm thay đổi duty cycle
void set_duty_cycle(int level) {
    uint32_t duty = 0;
    switch (level) {
        case 1:
            duty = 8192; // 33% duty cycle
            printf("Setting duty cycle to 33%%\n");
            break;
        case 2:
            duty = 16384; // 66% duty cycle
            printf("Setting duty cycle to 66%%\n");
            break;
        case 3:
            duty = 8191 * 2; // 100% duty cycle
            printf("Setting duty cycle to 100%%\n");
            break;
        default:
            printf("Invalid level! Please use 1, 2, or 3.\n");
            return;
    }
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}


// Main application entry point
void app_main(void) {
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    lcd_init();
    lcd_clear();

    // Example 1: Display static text
    lcd_put_cur(0, 0);
    lcd_send_string("Hello World!");

    lcd_put_cur(1, 0);
    lcd_send_string("from ESP32");

    // // Example 2: Display a formatted value
    // char buffer[16];
    // float num = 1234.56;

    // lcd_clear();  // Clear before displaying new content
    // sprintf(buffer, "Val=%.2f", num);  // Format float to 2 decimal places
    // lcd_put_cur(0, 0);
    // lcd_send_string(buffer);



    //Output bơm
    // Khởi tạo LEDC
    init_ledc();

    // Thay đổi duty cycle dựa trên tham số truyền vào
    set_duty_cycle(1); // 33%
    vTaskDelay(pdMS_TO_TICKS(3000)); // Delay 1 giây

    set_duty_cycle(2); // 66%
    vTaskDelay(pdMS_TO_TICKS(3000)); // Delay 1 giây

    set_duty_cycle(3); // 100%
    vTaskDelay(pdMS_TO_TICKS(3000)); // Delay 1 giây
}