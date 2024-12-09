#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c_eeprom.h"
#include "oneshot_read_main.h"
void app_main(void) {
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

    adc_cali_handle_t cali_handle = NULL;
    bool calibrated = adc_calibration_init(ADC_UNIT_1, ADC_CHANNEL_4, ADC_ATTEN_DB_12, &cali_handle);

    adc_configure_channel(&adc_handle, ADC_CHANNEL_4, ADC_ATTEN_DB_12);

    while (1) {
        adc_read_data(adc_handle, ADC_CHANNEL_4, cali_handle);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    adc_calibration_deinit(cali_handle);
    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc_handle));
}
