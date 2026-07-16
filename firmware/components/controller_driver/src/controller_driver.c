#include "controller_driver.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include <stdlib.h>

static QueueHandle_t event_queue = NULL;
static adc_oneshot_unit_handle_t adc_handle = NULL;

static void IRAM_ATTR gpio_isr_handler(void *arg) {
  uint32_t gpio_num = (uint32_t)arg;
  xQueueSendFromISR(event_queue, &gpio_num, NULL);
}

static void gpio_init(void) {
  // joystick

  adc_oneshot_unit_init_cfg_t init_config = {
      .unit_id = ADC_UNIT_1,
      .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

  adc_oneshot_chan_cfg_t chan_config = {
      .bitwidth = ADC_BITWIDTH_12,
      .atten = ADC_ATTEN_DB_12,
  };

  ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, CONTROLLER_JOYSTICK_X,
                                             &chan_config));
  ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, CONTROLLER_JOYSTICK_Y,
                                             &chan_config));
  ESP_ERROR_CHECK(
      adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_0, &chan_config));

  // buttons

  gpio_config_t io_conf = {.pin_bit_mask = INPUT_PIN_BITMASK,
                           .mode = GPIO_MODE_INPUT,
                           .pull_up_en = GPIO_PULLUP_ENABLE,
                           .pull_down_en = GPIO_PULLDOWN_DISABLE,
                           .intr_type = GPIO_INTR_NEGEDGE};
  gpio_config(&io_conf);

  event_queue = xQueueCreate(10, sizeof(uint32_t));

  gpio_install_isr_service(0);
  gpio_isr_handler_add(CONTROLLER_BUTTON_A, gpio_isr_handler,
                       (void *)CONTROLLER_BUTTON_A);
  gpio_isr_handler_add(CONTROLLER_BUTTON_B, gpio_isr_handler,
                       (void *)CONTROLLER_BUTTON_B);
}

static void joystick_task(void *arg) {
  Controller_t *controller = (Controller_t *)arg;
  int x_raw_val = 0;
  int y_raw_val = 0;

  while (1) {
    ESP_ERROR_CHECK(
        adc_oneshot_read(adc_handle, CONTROLLER_JOYSTICK_X, &x_raw_val));
    ESP_ERROR_CHECK(
        adc_oneshot_read(adc_handle, CONTROLLER_JOYSTICK_Y, &y_raw_val));

    int dx = x_raw_val - CONTROLLER_JOYSTICK_CENTER;
    int dy = y_raw_val - CONTROLLER_JOYSTICK_CENTER;

    if (abs(dx) < CONTROLLER_JOYSTICK_DEADZONE &&
        abs(dy) < CONTROLLER_JOYSTICK_DEADZONE) {
      controller->is_moving = false;
    } else if (abs(dx) > abs(dy)) {
      controller->last_direction =
          dx > 0 ? CONTROLLER_DIRECTION_LEFT : CONTROLLER_DIRECTION_RIGHT;
      printf("%s", dx > 0 ? "LEFT" : "RIGHT");
      controller->is_moving = true;
    } else {
      controller->last_direction =
          dy > 0 ? CONTROLLER_DIRECTION_DOWN : CONTROLLER_DIRECTION_UP;
      printf("%s", dx > 0 ? "DOWN" : "LEFT");
      controller->is_moving = true;
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

static void button_task(void *arg) {
  uint32_t io_num;

  // Create tracking variables to store the last valid click timestamp for
  // each button
  static int64_t last_press_time_a = 0;
  static int64_t last_press_time_b = 0;

  while (1) {
    if (xQueueReceive(event_queue, &io_num, portMAX_DELAY)) {

      // Get the current system runtime up-time in milliseconds
      int64_t current_time = esp_timer_get_time() / 1000;

      switch (io_num) {

      case CONTROLLER_BUTTON_A:
        if ((current_time - last_press_time_a) > CONTROLLER_DEBOUNCE_TIME_MS) {
          if (gpio_get_level(io_num) == 0) {
            printf("EVENT: Button A Pressed!\n");
            last_press_time_a = current_time;
          }
        }
        break;

      case CONTROLLER_BUTTON_B:
        if ((current_time - last_press_time_b) > CONTROLLER_DEBOUNCE_TIME_MS) {
          if (gpio_get_level(io_num) == 0) {
            printf("EVENT: Button B Pressed!\n");
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

void Controller_Init(Controller_t *controller) {
  gpio_init();
  xTaskCreate(joystick_task, "joystick_task", 2048, controller, 4, NULL);
  xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
}