#ifndef __CONTROLS_H_
#define __CONTROLS_H_

#include <stdbool.h>

#define CONTROLS_BUTTON_A (1) // D1
#define CONTROLS_BUTTON_B (0) // D0
#define CONTROLS_BUTTON_UP (20) // D7
#define CONTROLS_BUTTON_DOWN (18) // D8
#define CONTROLS_BUTTON_LEFT (19) // D9
#define CONTROLS_BUTTON_RIGHT (17) // D10
#define CONTROLS_BUTTON_CMD (16) // D6
#define CONTROLS_DEBOUNCE_MS 200

// Bitmask configuration helper
#define INPUT_PIN_BITMASK                                                                          \
    ((1ULL << CONTROLS_BUTTON_B) | (1ULL << CONTROLS_BUTTON_A) | (1ULL << CONTROLS_BUTTON_UP)      \
     | (1ULL << CONTROLS_BUTTON_DOWN) | (1ULL << CONTROLS_BUTTON_LEFT)                             \
     | (1ULL << CONTROLS_BUTTON_RIGHT) | (1ULL << CONTROLS_BUTTON_CMD))

typedef enum {
    CONTROLS_DIRECTION_UP = 0,
    CONTROLS_DIRECTION_DOWN,
    CONTROLS_DIRECTION_LEFT,
    CONTROLS_DIRECTION_RIGHT,
} Controls_Directions;

typedef struct
{
    Controls_Directions last_direction;
    bool is_moving;
} Controls_t;

void Controls_Init(Controls_t *controls);

#endif // __CONTROLS_DRIVER_H_