// #include "nvs_flash.h"
// #include <stdio.h>
//
#include "main.h"
#include "controller_driver.h"
#include "display.h"
#include "esp_err.h"
#include "esp_log.h"
#include "sound_driver.h"
#include "tracks.h"

static const char *TAG = "GAME_TEST";

App_t app;

void app_main(void) {
  esp_log_level_set("i2c", ESP_LOG_DEBUG);

  Display_Init(&app.display);
  MusicPlayer_Init(&app.music_player);
  Controller_Init(&app.controls);

  MusicPlayer_PlayTrack(&mario);

  Display_DrawCirc(&app.display, 10, 10, 10, false);
  Display_Flush(&app.display);

  while (1) {

    // Execute your game logic here
    // Update_Game_Physics();

    // Draw directly to your OLED screen here
    // Display_Render_Frame(&app.display);

    // Match this sleep to your target frame-rate (e.g., 33ms delay = ~30 FPS)
    vTaskDelay(pdMS_TO_TICKS(33));
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