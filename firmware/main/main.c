// #include "nvs_flash.h"
// #include <stdio.h>
//
#include "main.h"
#include "controls.h"
#include "device_system.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "game_engine.h"
#include "tracks.h"

static const char *TAG = "GAME_TEST";

static float old_voltage;
static float new_voltage;

static App_t app;

void app_main(void) {
  DSys_Init();
  GEng_Init();

  GEng_Sound_PlayTrack(&mario);
  GEnd_Dsp_Clear();

  while (1) {
    // GEng_HandleInput();
    // GEng_Display_Render();
    // GEng_CleanUp();
    // vTaskDelay(pdMS_TO_TICKS(33));
    old_voltage = new_voltage;
    GEng_Display_ClearFloat(20, 20, old_voltage, 2);

    new_voltage = DSys_Battery_Read();

    GEng_Display_DrawFloat(20, 20, new_voltage, 2, true);
    GEng_Display_Flush();

    vTaskDelay(pdMS_TO_TICKS(5000));
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