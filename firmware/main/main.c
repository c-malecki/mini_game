#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "galaga.h"
#include "game_engine.h"

static float old_voltage;
static float new_voltage;

void app_main(void) {
  ESP_LOGI("Main", "start");
  Player_t player;
  player.sprite = &SPRITE_PLAYER_SHIP;
  GEng_Init(player);
  ESP_LOGI("Main", "GEng_Init complete");
  // GEng_GAME_SetPlayer(player);

  while (1) {
    GEng_LOOP_HandleInput();
    GEng_LOOP_Render();
    vTaskDelay(pdMS_TO_TICKS(33));

    // old_voltage = new_voltage;
    // GEng_Display_ClearFloat(20, 20, old_voltage, 2);

    // new_voltage = DSys_Battery_Read();

    // GEng_display_drawFloat(20, 20, new_voltage, 2, true);
    // GEng_Display_Flush();

    // vTaskDelay(pdMS_TO_TICKS(5000));
  }
}