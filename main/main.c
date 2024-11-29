#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "adc_light_sensor.h"       // Header cho cảm biến ánh sáng
#include "moisture.h"    // Header cho cảm biến độ ẩm

#define SENSOR_1_PIN 4  // Chân GPIO cho cảm biến ánh sáng
#define SENSOR_2_PIN 5  // Chân GPIO cho cảm biến độ ẩm
#define SENSOR_3_PIN 6  // Chân GPIO cho cảm biến giả lập (hoặc cảm biến khác)

static const char *TAG = "SENSOR_TASKS";

// Task cho cảm biến ánh sáng
void sensor_1_task(void *pvParameters) {
    while (1) {
        int light_adc, light_voltage;
        light_read(&light_adc, &light_voltage);
        ESP_LOGI(TAG, "Light Sensor: ADC=%d, Voltage=%dmV", light_adc, light_voltage);
        vTaskDelay(pdMS_TO_TICKS(1000));  // Đọc mỗi giây
    }
}

// Task cho cảm biến độ ẩm
void sensor_2_task(void *pvParameters) {
    while (1) {
        int moisture_adc, moisture_voltage;
        moisture_read(&moisture_adc, &moisture_voltage);
        ESP_LOGI(TAG, "Moisture Sensor: ADC=%d, Voltage=%dmV", moisture_adc, moisture_voltage);
        vTaskDelay(pdMS_TO_TICKS(2000));  // Đọc mỗi 2 giây
    }
}

// Task cho cảm biến giả lập (hoặc cảm biến khác)
void sensor_3_task(void *pvParameters) {
    while (1) {
        int data = rand() % 100;  // Trả về giá trị ngẫu nhiên (giả lập)
        ESP_LOGI(TAG, "Sensor 3 Data: %d", data);
        vTaskDelay(pdMS_TO_TICKS(500));  // Đọc mỗi 0.5 giây
    }
}

void app_main() {
    ESP_LOGI(TAG, "Initializing tasks...");

    // Tạo các task cho từng cảm biến
    xTaskCreate(sensor_1_task, "Light Sensor Task", 2048, NULL, 5, NULL);
    xTaskCreate(sensor_2_task, "Moisture Sensor Task", 2048, NULL, 5, NULL);
    xTaskCreate(sensor_3_task, "Sensor 3 Task", 2048, NULL, 5, NULL);

    ESP_LOGI(TAG, "Tasks started!");
}
