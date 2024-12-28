#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "ledc_library.h"

// Hàm khởi tạo LEDC
void ledc_init(void) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .freq_hz          = LEDC_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .gpio_num       = LEDC_OUTPUT_IO,
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER,
        .duty           = 0,
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

// Hàm lấy giá trị duty cycle
uint32_t get_duty_cycle(int level) {
    switch (level) {
        case 1:
            return 8192; // 33% duty cycle
        case 2:
            return 16384; // 66% duty cycle
        case 3:
            return 24576; // 100% duty cycle
        default:
            printf("Invalid level! Please use 1, 2, or 3.\n");
            return 0;
    }
}

// Hàm cài đặt duty cycle
void set_duty_cycle(int level) { //Level 1-2-3
    uint32_t duty = get_duty_cycle(level);
    if (duty == 0) {
        // Trường hợp không hợp lệ
        return;
    }

    printf("Setting duty cycle to %d%%\n", (level * 33));
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}


// #include <stdio.h>
// #include <unistd.h> // for usleep
// #include "ledc_library.h"

// void main_program() {
//     // Khởi tạo LEDC
//     ledc_init();

//     // Thay đổi duty cycle
//     set_duty_cycle(1);
//     usleep(2000000); // Giữ 2 giây
//     set_duty_cycle(2);
//     usleep(2000000); // Giữ 2 giây
//     set_duty_cycle(3);
// }

// int main() {
//     main_program();
//     return 0;
// }
