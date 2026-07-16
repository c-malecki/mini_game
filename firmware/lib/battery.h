#ifndef __BATTERY_H_
#define __BATTERY_H_

#include "esp_adc/adc_oneshot.h"

#define BATTERY_ADC_CH ADC_CHANNEL_2
#define ADC_MAX_VAL 4095.0f // 12-bit max
#define VOLTAGE_DIVIDER_FACTOR 2.0f // Adjust based on your breadboard resistors
#define BAT_SAMPLE_COUNT 8

typedef struct
{
    adc_oneshot_unit_handle_t adc_handle;
    float voltage;
} Battery_t;

void Battery_Init(Battery_t *battery, adc_oneshot_unit_handle_t adc_handle);

#endif // __BATTERY_H_