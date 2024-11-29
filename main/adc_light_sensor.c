#include "adc_light_sensor.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

#define LIGHT_SENSOR_PIN ADC_CHANNEL_6  // Chỉnh lại channel của bạn
#define DEFAULT_VREF 1100

static adc_cali_handle_t adc_chars;
static adc_oneshot_unit_handle_t adc1_handle;

void light_init() {
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config, &adc1_handle);

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    adc_oneshot_config_channel(adc1_handle, LIGHT_SENSOR_PIN, &config);

    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    adc_cali_create_scheme_line_fitting(&cali_config, &adc_chars);
}

void light_read(int *adc_value, int *voltage) {
    adc_oneshot_read(adc1_handle, LIGHT_SENSOR_PIN, adc_value);
    adc_cali_raw_to_voltage(adc_chars, *adc_value, voltage);
}
