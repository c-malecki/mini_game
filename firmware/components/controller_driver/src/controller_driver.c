#include "controller_driver.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"

static void joystick_task(void *arg) {
  Controller_t *controller = (Controller_t *)arg;
  int x_raw_val = 0;
  int y_raw_val = 0;

  while (1) {
    ESP_ERROR_CHECK(adc_oneshot_read(controller->adc1_handle,
                                     CONTROLLER_JOYSTICK_X, &x_raw_val));
    ESP_ERROR_CHECK(adc_oneshot_read(controller->adc1_handle,
                                     CONTROLLER_JOYSTICK_Y, &y_raw_val));

    if (x_raw_val < CONTROLLER_JOYSTICK_THRESHOLD_LOW) {
      printf("Direction: Left\n");
      controller->last_direction = CONTROLLER_DIRECTION_LEFT;
    } else if (x_raw_val > CONTROLLER_JOYSTICK_THRESHOLD_HIGH) {
      printf("Direction: Right\n");
      controller->last_direction = CONTROLLER_DIRECTION_RIGHT;
    }

    if (y_raw_val < CONTROLLER_JOYSTICK_THRESHOLD_LOW) {
      printf("Direction: Up\n");
      controller->last_direction = CONTROLLER_DIRECTION_UP;
    } else if (y_raw_val > CONTROLLER_JOYSTICK_THRESHOLD_HIGH) {
      printf("Direction: Down\n");
      controller->last_direction = CONTROLLER_DIRECTION_DOWN;
    }
    controller->is_moving = true;

    // Run checking cycle at 100ms intervals to keep serial terminal readable
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// static void button_task(void *arg) {
//   uint32_t io_num;

//   // Create tracking variables to store the last valid click timestamp for
//   each
//   // button
//   static int64_t last_press_time_start = 0;
//   int64_t last_press_time_select = 0;
//   static int64_t last_press_time_a = 0;
//   static int64_t last_press_time_b = 0;

//   while (1) {
//     if (xQueueReceive(event_queue, &io_num, portMAX_DELAY)) {

//       // Get the current system runtime up-time in milliseconds
//       int64_t current_time = esp_timer_get_time() / 1000;

//       switch (io_num) {
//       case CONTROLLER_BUTTON_START:
//         // Only trigger if enough time has passed since the last confirmed
//         press if ((current_time - last_press_time_start) >
//             CONTROLLER_DEBOUNCE_TIME_MS) {

//           // Double check the physical pin state to ensure it's a real press
//           // (LOW)
//           if (gpio_get_level(io_num) == 0) {
//             printf("EVENT: Button Start Pressed!\n");
//             last_press_time_start = current_time; // Update the timestamp
//           }
//         }
//         break;

//       case CONTROLLER_BUTTON_SELECT:
//         // Only trigger if enough time has passed since the last confirmed
//         press if ((current_time - last_press_time_select) >
//             CONTROLLER_DEBOUNCE_TIME_MS) {

//           // Double check the physical pin state to ensure it's a real press
//           // (LOW)
//           if (gpio_get_level(io_num) == 0) {
//             printf("EVENT: Button Select Pressed!\n");
//             last_press_time_select = current_time; // Update the timestamp
//           }
//         }
//         break;

//       case CONTROLLER_BUTTON_A:
//         if ((current_time - last_press_time_a) > CONTROLLER_DEBOUNCE_TIME_MS)
//         {
//           if (gpio_get_level(io_num) == 0) {
//             printf("EVENT: Button A Pressed!\n");
//             last_press_time_a = current_time;
//           }
//         }
//         break;

//       case CONTROLLER_BUTTON_B:
//         if ((current_time - last_press_time_b) > CONTROLLER_DEBOUNCE_TIME_MS)
//         {
//           if (gpio_get_level(io_num) == 0) {
//             printf("EVENT: Button B Pressed!\n");
//             last_press_time_b = current_time;
//           }
//         }
//         break;

//       default:
//         break;
//       }
//     }
//   }
// }

void Controller_Task(void) {
  xTaskCreate(joystick_task, "joystick_task", 2048, NULL, 4, NULL);
  // xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
}