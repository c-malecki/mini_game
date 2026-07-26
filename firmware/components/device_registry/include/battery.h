#ifndef __BATTERY_H_
#define __BATTERY_H_

#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"

#define BATTERY_ADC_CH ADC_CHANNEL_2
#define ADC_MAX_VAL 4095.0f // 12-bit max
#define VOLTAGE_DIVIDER_FACTOR 2.0f // Adjust based on your breadboard resistors
#define BAT_SAMPLE_COUNT 8

typedef struct
{
    float voltage;
    // TODO:
    bool is_charging;
} Battery_t;

void Battery_Init(void);

void Battery_Get(Battery_t *battery);
void Battery_Set(float new_voltage, bool is_charging);

#endif // __BATTERY_H_