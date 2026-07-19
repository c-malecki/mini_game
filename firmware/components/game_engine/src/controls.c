#include "controls.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include <stdint.h>
#include <stdlib.h>

static QueueHandle_t event_queue = NULL;
static void button_task(void *arg);
static Controls_Buttons pending_btn;
static Controls_Directions pending_direction;

static void IRAM_ATTR gpio_isr_handler(void *arg) {
  uint32_t gpio_num = (uint32_t)arg;
  xQueueSendFromISR(event_queue, &gpio_num, NULL);
}

void Controls_Init(Controls_t *controls) {
  pending_btn = CONTROLS_BUTTON_NONE;
  pending_direction = CONTROLS_DIRECTION_NONE;

  gpio_config_t io_cfg = {.pin_bit_mask = INPUT_PIN_BITMASK,
                          .mode = GPIO_MODE_INPUT,
                          .pull_up_en = GPIO_PULLUP_ENABLE,
                          .pull_down_en = GPIO_PULLDOWN_DISABLE,
                          .intr_type = GPIO_INTR_NEGEDGE};
  gpio_config(&io_cfg);

  event_queue = xQueueCreate(10, sizeof(uint32_t));

  gpio_install_isr_service(0);

  gpio_isr_handler_add(CONTROLS_A_PIN, gpio_isr_handler,
                       (void *)CONTROLS_A_PIN);
  gpio_isr_handler_add(CONTROLS_B_PIN, gpio_isr_handler,
                       (void *)CONTROLS_B_PIN);

  gpio_isr_handler_add(CONTROLS_UP_PIN, gpio_isr_handler,
                       (void *)CONTROLS_UP_PIN);
  gpio_isr_handler_add(CONTROLS_DOWN_PIN, gpio_isr_handler,
                       (void *)CONTROLS_DOWN_PIN);
  gpio_isr_handler_add(CONTROLS_LEFT_PIN, gpio_isr_handler,
                       (void *)CONTROLS_LEFT_PIN);
  gpio_isr_handler_add(CONTROLS_RIGHT_PIN, gpio_isr_handler,
                       (void *)CONTROLS_RIGHT_PIN);

  gpio_isr_handler_add(CONTROLS_CMD_PIN, gpio_isr_handler,
                       (void *)CONTROLS_CMD_PIN);

  xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
}

Controls_Buttons Controls_GetPendingBtn(void) {
  Controls_Buttons btn = pending_btn;
  return btn;
}

Controls_Directions Controls_GetPendingDirection(void) {
  Controls_Directions dir = pending_direction;
  return dir;
}

void Controls_CleanUp(void) {
  pending_direction = CONTROLS_DIRECTION_NONE;
  pending_btn = CONTROLS_BUTTON_NONE;
}

static void button_task(void *arg) {
  uint32_t io_num;

  // Create tracking variables to store the last valid click timestamp for
  // each button
  static int64_t last_press_time_a = 0;
  static int64_t last_press_time_b = 0;
  static int64_t last_press_time_up = 0;
  static int64_t last_press_time_down = 0;
  static int64_t last_press_time_left = 0;
  static int64_t last_press_time_right = 0;
  static int64_t last_press_time_cmd = 0;

  while (1) {
    if (xQueueReceive(event_queue, &io_num, portMAX_DELAY)) {

      // Get the current system runtime up-time in milliseconds
      int64_t current_time = esp_timer_get_time() / 1000;

      switch (io_num) {

      case CONTROLS_A_PIN:
        if ((current_time - last_press_time_a) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            pending_btn = CONTROLS_BUTTON_A;
            printf("A\n");
            last_press_time_a = current_time;
          }
        }
        break;

      case CONTROLS_B_PIN:
        if ((current_time - last_press_time_b) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            pending_btn = CONTROLS_BUTTON_B;
            printf("B\n");
            last_press_time_b = current_time;
          }
        }
        break;

      case CONTROLS_CMD_PIN:
        if ((current_time - last_press_time_cmd) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            pending_btn = CONTROLS_CMD_PIN;
            printf("CMD\n");
            last_press_time_b = current_time;
          }
        }
        break;

      case CONTROLS_UP_PIN:
        if ((current_time - last_press_time_up) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            pending_direction = CONTROLS_DIRECTION_UP;
            printf("UP\n");
            last_press_time_b = current_time;
          }
        }
        break;

      case CONTROLS_DOWN_PIN:
        if ((current_time - last_press_time_down) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            pending_direction = CONTROLS_DIRECTION_DOWN;
            printf("DOWN\n");
            last_press_time_b = current_time;
          }
        }
        break;

      case CONTROLS_LEFT_PIN:
        if ((current_time - last_press_time_left) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            pending_direction = CONTROLS_DIRECTION_LEFT;
            printf("LEFT\n");
            last_press_time_b = current_time;
          }
        }
        break;

      case CONTROLS_RIGHT_PIN:
        if ((current_time - last_press_time_right) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            pending_direction = CONTROLS_DIRECTION_RIGHT;
            printf("RIGHT\n");
            last_press_time_b = current_time;
          }
        }
        break;

      default:
        break;
      }
    }
  }
}