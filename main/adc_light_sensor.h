#ifndef ADC_LIGHT_SENSOR_H
#define ADC_LIGHT_SENSOR_H

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

// Định nghĩa chân cảm biến ánh sáng
#define LIGHT_SENSOR_PIN ADC_CHANNEL_6 // Chỉnh lại channel phù hợp
#define DEFAULT_VREF 1100

// Hàm khởi tạo ADC
void init_adc(void);

// Hàm đọc giá trị ADC và chuyển đổi ra điện áp
void light_read(int *adc_value, int *voltage);


#endif // ADC_LIGHT_SENSOR_H
