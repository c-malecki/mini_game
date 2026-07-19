#ifndef __CONTROLS_H_
#define __CONTROLS_H_

#include <stdbool.h>
#include <stdint.h>

#define CONTROLS_A_PIN (1) // D1
#define CONTROLS_B_PIN (0) // D0
#define CONTROLS_UP_PIN (20) // D7
#define CONTROLS_DOWN_PIN (18) // D8
#define CONTROLS_LEFT_PIN (19) // D9
#define CONTROLS_RIGHT_PIN (17) // D10
#define CONTROLS_CMD_PIN (16) // D6

#define CONTROLS_DEBOUNCE_MS 200

// Bitmask configuration helper
#define INPUT_PIN_BITMASK                                                                          \
    ((1ULL << CONTROLS_B_PIN) | (1ULL << CONTROLS_A_PIN) | (1ULL << CONTROLS_UP_PIN)               \
     | (1ULL << CONTROLS_DOWN_PIN) | (1ULL << CONTROLS_LEFT_PIN) | (1ULL << CONTROLS_RIGHT_PIN)    \
     | (1ULL << CONTROLS_CMD_PIN))

typedef enum {
    CONTROLS_DIRECTION_NONE = 0,
    CONTROLS_DIRECTION_UP,
    CONTROLS_DIRECTION_DOWN,
    CONTROLS_DIRECTION_LEFT,
    CONTROLS_DIRECTION_RIGHT,
} Controls_Directions;

typedef enum {
    CONTROLS_BUTTON_NONE = 0,
    CONTROLS_BUTTON_A,
    CONTROLS_BUTTON_B,
    CONTROLS_BUTTON_CMD
} Controls_Buttons;

typedef struct
{
    Controls_Directions pending_direction;
    Controls_Buttons pending_btn;
    bool is_moving;
} Controls_t;

void Controls_Init(Controls_t *controls);
Controls_Buttons Controls_GetPendingBtn(void);
Controls_Directions Controls_GetPendingDirection(void);
void Controls_CleanUp(void);

#endif // __CONTROLS_DRIVER_H_