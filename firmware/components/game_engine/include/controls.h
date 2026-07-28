#ifndef __CONTROLS_H_
#define __CONTROLS_H_

#include <stdbool.h>
#include <stdint.h>

#define CONTROLS_PIN_A (1) // D1
#define CONTROLS_PIN_B (0) // D0
#define CONTROLS_PIN_UP (20) // D7
#define CONTROLS_PIN_DOWN (18) // D8
#define CONTROLS_PIN_LEFT (19) // D9
#define CONTROLS_PIN_RIGHT (17) // D10
#define CONTROLS_PIN_CMD (16) // D6

#define CONTROLS_DEBOUNCE_MS 200

typedef void (*Controls_Button_CB_t)(void);

typedef enum {
    CONTROLS_BTN_B = 0,
    CONTROLS_BTN_A,
    CONTROLS_BTN_CMD,
    CONTROLS_BTN_UP,
    CONTROLS_BTN_DOWN,
    CONTROLS_BTN_LEFT,
    CONTROLS_BTN_RIGHT,
} Controls_Buttons;

#define INPUT_PIN_BITMASK                                                                          \
    ((1ULL << CONTROLS_PIN_B) | (1ULL << CONTROLS_PIN_A) | (1ULL << CONTROLS_PIN_UP)               \
     | (1ULL << CONTROLS_PIN_DOWN) | (1ULL << CONTROLS_PIN_LEFT) | (1ULL << CONTROLS_PIN_RIGHT)    \
     | (1ULL << CONTROLS_PIN_CMD))

typedef struct
{
    Controls_Button_CB_t on_press_cmd;
    Controls_Button_CB_t on_press_a;
    Controls_Button_CB_t on_press_b;
    Controls_Button_CB_t on_press_up;
    Controls_Button_CB_t on_press_down;
    Controls_Button_CB_t on_press_left;
    Controls_Button_CB_t on_press_right;
} Controls_t;

void Controls_Init(const Controls_t *controls);
void Controls_Register(const Controls_t *controls);

bool Controls_IsPinActive(int pin);
int64_t Controls_GetPinActiveMS(Controls_Buttons btn);

#endif // __CONTROLS_DRIVER_H_