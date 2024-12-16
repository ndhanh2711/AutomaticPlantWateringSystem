#ifndef ADC_LIBRARY_H
#define ADC_LIBRARY_H

#include "freertos/FreeRTOS.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

bool adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle);
void adc_calibration_deinit(adc_cali_handle_t handle);
void adc_configure_channel(adc_oneshot_unit_handle_t *adc_handle, adc_channel_t channel, adc_atten_t atten);
void adc_read_data(adc_oneshot_unit_handle_t adc_handle, adc_channel_t channel, adc_cali_handle_t cali_handle);

#endif // ADC_LIBRARY_H
