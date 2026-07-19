#include "controls.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include <stdlib.h>

static QueueHandle_t event_queue = NULL;
static void button_task(void *arg);

static void IRAM_ATTR gpio_isr_handler(void *arg) {
  uint32_t gpio_num = (uint32_t)arg;
  xQueueSendFromISR(event_queue, &gpio_num, NULL);
}

void Controls_Init(Controls_t *controls) {
  gpio_config_t io_cfg = {.pin_bit_mask = INPUT_PIN_BITMASK,
                          .mode = GPIO_MODE_INPUT,
                          .pull_up_en = GPIO_PULLUP_ENABLE,
                          .pull_down_en = GPIO_PULLDOWN_DISABLE,
                          .intr_type = GPIO_INTR_NEGEDGE};
  gpio_config(&io_cfg);

  event_queue = xQueueCreate(10, sizeof(uint32_t));

  gpio_install_isr_service(0);

  gpio_isr_handler_add(CONTROLS_BUTTON_A, gpio_isr_handler,
                       (void *)CONTROLS_BUTTON_A);
  gpio_isr_handler_add(CONTROLS_BUTTON_B, gpio_isr_handler,
                       (void *)CONTROLS_BUTTON_B);

  gpio_isr_handler_add(CONTROLS_BUTTON_UP, gpio_isr_handler,
                       (void *)CONTROLS_BUTTON_UP);
  gpio_isr_handler_add(CONTROLS_BUTTON_DOWN, gpio_isr_handler,
                       (void *)CONTROLS_BUTTON_DOWN);
  gpio_isr_handler_add(CONTROLS_BUTTON_LEFT, gpio_isr_handler,
                       (void *)CONTROLS_BUTTON_LEFT);
  gpio_isr_handler_add(CONTROLS_BUTTON_RIGHT, gpio_isr_handler,
                       (void *)CONTROLS_BUTTON_RIGHT);

  gpio_isr_handler_add(CONTROLS_BUTTON_CMD, gpio_isr_handler,
                       (void *)CONTROLS_BUTTON_CMD);

  xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
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

      case CONTROLS_BUTTON_A:
        if ((current_time - last_press_time_a) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            printf("EVENT: Button A Pressed!\n");
            last_press_time_a = current_time;
          }
        }
        break;

      case CONTROLS_BUTTON_B:
        if ((current_time - last_press_time_b) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            printf("EVENT: Button B Pressed!\n");
            last_press_time_b = current_time;
          }
        }
        break;

      case CONTROLS_BUTTON_UP:
        if ((current_time - last_press_time_up) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            printf("EVENT: Button UP Pressed!\n");
            last_press_time_b = current_time;
          }
        }
        break;

      case CONTROLS_BUTTON_DOWN:
        if ((current_time - last_press_time_down) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            printf("EVENT: Button DOWN Pressed!\n");
            last_press_time_b = current_time;
          }
        }
        break;

      case CONTROLS_BUTTON_LEFT:
        if ((current_time - last_press_time_left) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            printf("EVENT: Button LEFT Pressed!\n");
            last_press_time_b = current_time;
          }
        }
        break;

      case CONTROLS_BUTTON_RIGHT:
        if ((current_time - last_press_time_right) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            printf("EVENT: Button RIGHT Pressed!\n");
            last_press_time_b = current_time;
          }
        }
        break;

      case CONTROLS_BUTTON_CMD:
        if ((current_time - last_press_time_cmd) > CONTROLS_DEBOUNCE_MS) {
          if (gpio_get_level(io_num) == 0) {
            printf("EVENT: Button CMD Pressed!\n");
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