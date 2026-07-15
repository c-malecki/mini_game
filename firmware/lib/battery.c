#include "battery.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"

float Battery_ReadVoltage(adc_oneshot_unit_handle_t shared_adc_handle) {
  int raw_adc_value = 0;
  long long adc_accumulator = 0;

  // Sample multiple times to overcome high-impedance source drift
  for (int i = 0; i < BAT_SAMPLE_COUNT; i++) {
    ESP_ERROR_CHECK(
        adc_oneshot_read(shared_adc_handle, ADC_CHANNEL_0, &raw_adc_value));
    adc_accumulator += raw_adc_value;
  }

  // Find the mathematical average of our raw samples
  float average_raw_adc = (float)adc_accumulator / BAT_SAMPLE_COUNT;

  // Convert raw reading to the measured voltage present *at the pin* (0.0V
  // to 3.3V)
  float measured_pin_voltage = (average_raw_adc / ADC_MAX_VAL) * 3.3f;

  // Multiply by 2.0 to restore the full cell voltage
  float true_battery_voltage = measured_pin_voltage * VOLTAGE_DIVIDER_FACTOR;

  return true_battery_voltage;
}

static void battery_monitor_task(void *arg) {
  Battery_t *battery = (Battery_t *)arg;
  while (1) {
    if (battery->adc1_handle != NULL) {
      int raw_adc_value = 0;
      long long adc_accumulator = 0;

      for (int i = 0; i < 8; i++) {
        adc_oneshot_read(battery->adc1_handle, ADC_CHANNEL_0, &raw_adc_value);
        adc_accumulator += raw_adc_value;
      }

      float avg_raw = (float)adc_accumulator / 8.0f;
      float pin_v = (avg_raw / 4095.0f) * 3.3f;

      battery->voltage = pin_v * 2.0f;
      printf("Battery Voltage: %.2f!\n", battery->voltage);
    }

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void Battery_Task(void) {
  xTaskCreate(battery_monitor_task, // Function name
              "battery_task",       // Thread debug name
              2048,                 // Stack allocation size
              NULL,                 // Parameter inputs
              2,   // Low Priority (battery shifts are non-urgent)
              NULL // Thread handle reference
  );
}