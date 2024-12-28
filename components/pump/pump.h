#ifndef LEDC_LIBRARY_H
#define LEDC_LIBRARY_H

#include <stdint.h>

// Định nghĩa cấu hình LEDC
#define LEDC_TIMER          LEDC_TIMER_0
#define LEDC_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO      (15) // GPIO của LED
#define LEDC_CHANNEL        LEDC_CHANNEL_0
#define LEDC_FREQUENCY      (5000) // Tần số 5 kHz

// Khởi tạo LEDC
void ledc_init(void);

// Hàm lấy giá trị duty cycle theo mức
uint32_t get_duty_cycle(int level);

// Hàm cài đặt duty cycle
void set_duty_cycle(int level);

#endif // LEDC_LIBRARY_H
