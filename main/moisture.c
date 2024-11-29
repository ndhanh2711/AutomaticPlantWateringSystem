#include "moisture.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_log.h"

#define MOISTURE_SENSOR_PIN ADC_CHANNEL_7  // Chọn đúng channel cảm biến độ ẩm

static adc_cali_handle_t adc_chars;
static adc_oneshot_unit_handle_t adc1_handle;

void moisture_init() {
    // Khởi tạo ADC Unit
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config, &adc1_handle);

    // Cấu hình channel cho cảm biến độ ẩm
    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    adc_oneshot_config_channel(adc1_handle, MOISTURE_SENSOR_PIN, &config);

    // Khởi tạo cấu hình hiệu chỉnh
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    adc_cali_create_scheme_line_fitting(&cali_config, &adc_chars);
}

void moisture_read(int *adc_value, int *voltage) {
    adc_oneshot_read(adc1_handle, MOISTURE_SENSOR_PIN, adc_value);
    adc_cali_raw_to_voltage(adc_chars, *adc_value, voltage);
}
