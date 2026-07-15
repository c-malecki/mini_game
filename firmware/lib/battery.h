#ifndef __BATTERY_H_
#define __BATTERY_H_

#include "esp_adc/adc_oneshot.h"

#define ADC_MAX_VAL 4095.0f // 12-bit max
#define VOLTAGE_DIVIDER_FACTOR 2.0f // Adjust based on your breadboard resistors
#define BAT_SAMPLE_COUNT 8

typedef struct
{
    adc_oneshot_unit_handle_t adc1_handle;
    float voltage;
} Battery_t;

void Battery_Task(void);

#endif // __BATTERY_H_