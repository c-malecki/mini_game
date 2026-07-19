#include "battery.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"

static adc_oneshot_unit_handle_t adc_handle;
static float current_voltage;

static void battery_monitor_task(void *arg) {
  while (1) {
    if (adc_handle != NULL) {
      int raw_adc_value = 0;
      long long adc_accumulator = 0;

      for (int i = 0; i < 8; i++) {
        adc_oneshot_read(adc_handle, BATTERY_ADC_CH, &raw_adc_value);
        adc_accumulator += raw_adc_value;
      }

      float avg_raw = (float)adc_accumulator / 8.0f;
      float pin_v = (avg_raw / 4095.0f) * 3.3f;

      current_voltage = pin_v * 2.0f;
    }

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void Battery_Init(void) {
  adc_oneshot_unit_init_cfg_t init_config = {
      .unit_id = ADC_UNIT_1,
      .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

  adc_oneshot_chan_cfg_t chan_config = {
      .bitwidth = ADC_BITWIDTH_12,
      .atten = ADC_ATTEN_DB_12,
  };
  ESP_ERROR_CHECK(
      adc_oneshot_config_channel(adc_handle, BATTERY_ADC_CH, &chan_config));

  xTaskCreate(battery_monitor_task, "battery_task", 2048, NULL, 2, NULL);
}

float Battery_ReadV(void) {
  float cv = current_voltage;
  return cv;
}