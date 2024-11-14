#include <stdio.h>
#include "driver/esp_log.h"
#include "driver/adc_oneshot.h"
#include "driver/adc_cali.h"
#include "driver/adc_cali_schemes.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
static const char *TAG = "ADC";

void app_main(void)
{
    // Khởi tạo cấu hình cho ADC1
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_11,          // Chọn độ suy giảm tín hiệu
        .bitwidth = ADC_BITWIDTH_DEFAULT,  // Chọn độ phân giải mặc định
    };

    // Initialize ADC1
    esp_err_t ret;
    ret = adc_oneshot_new_unit(&init_config, &adc1_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Không thể khởi tạo ADC đơn lẻ.");
        return;
    }

    // Cấu hình kênh ADC
    ret = adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_6, &config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Không thể cấu hình kênh ADC.");
        return;
    }

    while (1) {
        // Đọc giá trị ADC
        int raw;
        ret = adc_oneshot_read(adc1_handle, ADC_CHANNEL_6, &raw);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Giá trị ADC: %d", raw);
        } else {
            ESP_LOGE(TAG, "Không thể đọc giá trị ADC.");
        }

        // Chờ 2 giây trước khi đọc lại
        vTaskDelay(pdMS_TO_TICKS(2000)); // 2000 ms = 2 seconds
    }

    // Dọn dẹp và giải phóng tài nguyên ADC (sẽ không thực hiện được do vòng lặp vô hạn)
    // Nếu muốn dọn dẹp, cần thêm điều kiện thoát vòng lặp.
    adc_oneshot_del_unit(adc1_handle);
}
