#include "controls.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include <stdint.h>
#include <stdlib.h>

static Controls_t active_controls = {0};
static QueueHandle_t event_queue = NULL;
static void button_task(void *arg);

static void IRAM_ATTR gpio_isr_handler(void *arg) {
  uint32_t gpio_num = (uint32_t)arg;
  xQueueSendFromISR(event_queue, &gpio_num, NULL);
}

static int64_t last_press_time_a = 0;
static int64_t last_press_time_b = 0;
static int64_t last_press_time_up = 0;
static int64_t last_press_time_down = 0;
static int64_t last_press_time_left = 0;
static int64_t last_press_time_right = 0;
static int64_t last_press_time_cmd = 0;

static int64_t a_press_start_ms = 0;
static int64_t b_press_start_ms = 0;
static int64_t up_press_start_ms = 0;
static int64_t down_press_start_ms = 0;
static int64_t left_press_start_ms = 0;
static int64_t right_press_start_ms = 0;
static int64_t cmd_press_start_ms = 0;

void Controls_Init(const Controls_t *controls) {
  gpio_config_t io_cfg = {.pin_bit_mask = INPUT_PIN_BITMASK,
                          .mode = GPIO_MODE_INPUT,
                          .pull_up_en = GPIO_PULLUP_ENABLE,
                          .pull_down_en = GPIO_PULLDOWN_DISABLE,
                          .intr_type = GPIO_INTR_NEGEDGE};
  gpio_config(&io_cfg);

  event_queue = xQueueCreate(10, sizeof(uint32_t));

  gpio_install_isr_service(0);

  gpio_isr_handler_add(CONTROLS_PIN_A, gpio_isr_handler,
                       (void *)CONTROLS_PIN_A);
  gpio_isr_handler_add(CONTROLS_PIN_B, gpio_isr_handler,
                       (void *)CONTROLS_PIN_B);

  gpio_isr_handler_add(CONTROLS_PIN_UP, gpio_isr_handler,
                       (void *)CONTROLS_PIN_UP);
  gpio_isr_handler_add(CONTROLS_PIN_DOWN, gpio_isr_handler,
                       (void *)CONTROLS_PIN_DOWN);
  gpio_isr_handler_add(CONTROLS_PIN_LEFT, gpio_isr_handler,
                       (void *)CONTROLS_PIN_LEFT);
  gpio_isr_handler_add(CONTROLS_PIN_RIGHT, gpio_isr_handler,
                       (void *)CONTROLS_PIN_RIGHT);

  gpio_isr_handler_add(CONTROLS_PIN_CMD, gpio_isr_handler,
                       (void *)CONTROLS_PIN_CMD);

  active_controls = *controls;

  xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
}

void Controls_Register(const Controls_t *controls) {
  active_controls = *controls;
}

bool Controls_IsPinActive(int pin) { return gpio_get_level(pin) == 0; }

int64_t Controls_GetPinActiveMS(Controls_Buttons btn) {
  int pin;
  int64_t press_start;

  switch (btn) {
  case CONTROLS_BTN_B:
    pin = CONTROLS_PIN_B;
    press_start = b_press_start_ms;
    break;
  case CONTROLS_BTN_A:
    pin = CONTROLS_PIN_A;
    press_start = a_press_start_ms;
    break;
  case CONTROLS_BTN_CMD:
    pin = CONTROLS_PIN_CMD;
    press_start = cmd_press_start_ms;
    break;
  case CONTROLS_BTN_UP:
    pin = CONTROLS_PIN_UP;
    press_start = up_press_start_ms;
    break;
  case CONTROLS_BTN_DOWN:
    pin = CONTROLS_PIN_DOWN;
    press_start = down_press_start_ms;
    break;
  case CONTROLS_BTN_LEFT:
    pin = CONTROLS_PIN_LEFT;
    press_start = left_press_start_ms;
    break;
  case CONTROLS_BTN_RIGHT:
    pin = CONTROLS_PIN_RIGHT;
    press_start = right_press_start_ms;
    break;
  default:
    return 0;
  }

  if (press_start == 0 || Controls_IsPinActive(pin) == false) {
    return 0;
  }

  return (esp_timer_get_time() / 1000) - press_start;
}

static void button_task(void *arg) {
  uint32_t io_num;

  while (1) {
    if (xQueueReceive(event_queue, &io_num, portMAX_DELAY)) {
      int64_t current_time = esp_timer_get_time() / 1000;

      switch (io_num) {

      case CONTROLS_PIN_A:
        if ((current_time - last_press_time_a) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            last_press_time_a = current_time;
            a_press_start_ms = current_time;
            if (active_controls.on_press_a != NULL) {
              active_controls.on_press_a();
            }
          }
        }
        break;

      case CONTROLS_PIN_B:
        if ((current_time - last_press_time_b) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            last_press_time_b = current_time;
            b_press_start_ms = current_time;
            if (active_controls.on_press_b != NULL) {
              active_controls.on_press_b();
            }
          }
        }
        break;

      case CONTROLS_PIN_CMD:
        if ((current_time - last_press_time_cmd) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            last_press_time_cmd = current_time;
            cmd_press_start_ms = current_time;
            if (active_controls.on_press_cmd != NULL) {
              active_controls.on_press_cmd();
            }
          }
        }
        break;

      case CONTROLS_PIN_UP:
        if ((current_time - last_press_time_up) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            last_press_time_up = current_time;
            up_press_start_ms = current_time;
            if (active_controls.on_press_up != NULL) {
              active_controls.on_press_up();
            }
          }
        }
        break;

      case CONTROLS_PIN_DOWN:
        if ((current_time - last_press_time_down) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            last_press_time_down = current_time;
            down_press_start_ms = current_time;
            if (active_controls.on_press_down != NULL) {
              active_controls.on_press_down();
            }
          }
        }
        break;

      case CONTROLS_PIN_LEFT:
        if ((current_time - last_press_time_left) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            last_press_time_left = current_time;
            left_press_start_ms = current_time;
            if (active_controls.on_press_left != NULL) {
              active_controls.on_press_left();
            }
          }
        }
        break;

      case CONTROLS_PIN_RIGHT:
        if ((current_time - last_press_time_right) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            last_press_time_right = current_time;
            right_press_start_ms = current_time;
            if (active_controls.on_press_right != NULL) {
              active_controls.on_press_right();
            }
          }
        }
        break;

      default:
        break;
      }
    }
  }
}