#ifndef __BATTERY_H_
#define __BATTERY_H_

#define BATTERY_ADC_CH ADC_CHANNEL_2
#define ADC_MAX_VAL 4095.0f // 12-bit max
#define VOLTAGE_DIVIDER_FACTOR 2.0f // Adjust based on your breadboard resistors
#define BAT_SAMPLE_COUNT 8

void Battery_Init(void);
float Battery_ReadV(void);

#endif // __BATTERY_H_