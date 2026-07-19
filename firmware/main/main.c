// #include "nvs_flash.h"
// #include <stdio.h>
//
#include "main.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "game_engine.h"
#include "tracks.h"

/*
#include "game.h"
#include <stdio.h>

static Player_t player = { .x = 64, .y = 32, .speed = 2 };
*/

static const char *TAG = "GAME_TEST";

static App_t app;

void app_main(void) {
  esp_log_level_set("i2c", ESP_LOG_DEBUG);

  Device_System_Init(&app.device_system);
  Game_Engine_Init(&app.game_engine);

  Game_Engine_PlayTrack(&mario);

  // Display_Flush(&app.display);

  while (1) {
    // Game_Update(&player, &app.controls);
    // Game_Render(&app.display, &player);
    vTaskDelay(pdMS_TO_TICKS(33));

    // Display_DrawFloat(&app.display, 20, 20, app.battery.voltage, 2, true);
    // Display_Flush(&app.display);

    // vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

/*
  app.data_mutex = xSemaphoreCreateMutex();
  assert(app.data_mutex != NULL);

  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
*/