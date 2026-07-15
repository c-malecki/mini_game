#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_

#include "freertos/FreeRTOS.h"
#include "esp_adc/adc_oneshot.h"
#include <stdbool.h>

#define CONTROLLER_JOYSTICK_X ADC_CHANNEL_1
#define CONTROLLER_JOYSTICK_Y ADC_CHANNEL_2
// Calibration thresholds based on 12-bit ADC (0 to 4095)
#define CONTROLLER_JOYSTICK_CENTER 2048
#define CONTROLLER_JOYSTICK_DEADZONE 500 // Prevent drifting near center

#define CONTROLLER_JOYSTICK_THRESHOLD_LOW                                                          \
    (CONTROLLER_JOYSTICK_CENTER - CONTROLLER_JOYSTICK_DEADZONE) // ~1548

#define CONTROLLER_JOYSTICK_THRESHOLD_HIGH                                                         \
    (CONTROLLER_JOYSTICK_CENTER + CONTROLLER_JOYSTICK_DEADZONE) // ~2548

#define CONTROLLER_BUTTON_START (18) // D10
#define CONTROLLER_BUTTON_SELECT (20) // D9
#define CONTROLLER_BUTTON_A (19) // D8
#define CONTROLLER_BUTTON_B (17) // D7
#define CONTROLLER_DEBOUNCE_TIME_MS 200

// Bitmask configuration helper
#define INPUT_PIN_BITMASK                                                                          \
    ((1ULL << CONTROLLER_BUTTON_B) | (1ULL << CONTROLLER_BUTTON_A)                                 \
     | (1ULL << CONTROLLER_BUTTON_SELECT) | (1ULL << CONTROLLER_BUTTON_START))

typedef enum {
    CONTROLLER_DIRECTION_UP = 0,
    CONTROLLER_DIRECTION_DOWN,
    CONTROLLER_DIRECTION_LEFT,
    CONTROLLER_DIRECTION_RIGHT,
} Controller_Directions;

typedef struct
{
    adc_oneshot_unit_handle_t adc1_handle;
    Controller_Directions last_direction;
    bool is_moving;
} Controller_t;

void Controller_Task(void);

#endif // __CONTROLLER_DRIVER_H_