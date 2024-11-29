#ifndef MOISTURE_H
#define MOISTURE_H

void moisture_init();                             // Khởi tạo cảm biến độ ẩm
void moisture_read(int *adc_value, int *voltage); // Đọc dữ liệu từ cảm biến độ ẩm

#endif
