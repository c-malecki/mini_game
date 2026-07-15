// #include "nvs_flash.h"
// #include <stdio.h>
//
#include "main.h"
// #include "controller_driver.h"
#include "display.h"
#include "esp_err.h"
#include "esp_log.h"
#include "sound_driver.h"
#include "tracks.h"

static const char *TAG = "GAME_TEST";

static App_t app;
static QueueHandle_t event_queue = NULL;
static adc_oneshot_unit_handle_t global_adc_handle = NULL;
static i2c_master_bus_handle_t global_i2c_handle = NULL;

static void app_init(void);

static void IRAM_ATTR gpio_isr_handler(void *arg) {
  uint32_t gpio_num = (uint32_t)arg;
  xQueueSendFromISR(event_queue, &gpio_num, NULL);
}

void app_main(void) {
  esp_log_level_set("i2c", ESP_LOG_DEBUG);

  app_init();

  Display_Init(&app.display, global_i2c_handle);
  // MusicPlayer_Init(&app.music_player);
  // Controller_Init(&app.controller);

  // MusicPlayer_PlayTrack(&mario);

  while (1) {

    // Execute your game logic here
    // Update_Game_Physics();

    // Draw directly to your OLED screen here
    // Display_Render_Frame(&app.display);

    // Match this sleep to your target frame-rate (e.g., 33ms delay = ~30 FPS)
    vTaskDelay(pdMS_TO_TICKS(33));
  }
}

void app_init(void) {
  // adc_oneshot_unit_init_cfg_t init_config = {
  //     .unit_id = ADC_UNIT_1,
  //     .ulp_mode = ADC_ULP_MODE_DISABLE,
  // };
  // ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &global_adc_handle));

  // adc_oneshot_chan_cfg_t chan_config = {
  //     .bitwidth = ADC_BITWIDTH_12,
  //     .atten = ADC_ATTEN_DB_12,
  // };

  // ESP_ERROR_CHECK(adc_oneshot_config_channel(
  //     global_adc_handle, CONTROLLER_JOYSTICK_X, &chan_config));
  // ESP_ERROR_CHECK(adc_oneshot_config_channel(
  //     global_adc_handle, CONTROLLER_JOYSTICK_Y, &chan_config));
  // ESP_ERROR_CHECK(adc_oneshot_config_channel(global_adc_handle,
  // ADC_CHANNEL_0,
  //                                            &chan_config));

  // gpio_config_t io_conf = {.pin_bit_mask = INPUT_PIN_BITMASK,
  //                          .mode = GPIO_MODE_INPUT,
  //                          .pull_up_en = GPIO_PULLUP_ENABLE,
  //                          .pull_down_en = GPIO_PULLDOWN_DISABLE,
  //                          .intr_type = GPIO_INTR_NEGEDGE};
  // gpio_config(&io_conf);

  // event_queue = xQueueCreate(10, sizeof(uint32_t));

  // gpio_install_isr_service(0);

  // gpio_isr_handler_add(CONTROLLER_BUTTON_START, gpio_isr_handler,
  //                      (void *)CONTROLLER_BUTTON_START);
  // gpio_isr_handler_add(CONTROLLER_BUTTON_SELECT, gpio_isr_handler,
  //                      (void *)CONTROLLER_BUTTON_SELECT);
  // gpio_isr_handler_add(CONTROLLER_BUTTON_A, gpio_isr_handler,
  //                      (void *)CONTROLLER_BUTTON_A);
  // gpio_isr_handler_add(CONTROLLER_BUTTON_B, gpio_isr_handler,
  //                      (void *)CONTROLLER_BUTTON_B);

  i2c_master_bus_config_t i2c_bus_cfg = {
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .i2c_port = I2C_NUM_0,
      .sda_io_num = DISPLAY_SDA_PIN,
      .scl_io_num = DISPLAY_SCL_PIN,
      .glitch_ignore_cnt = 7,
      .flags.enable_internal_pullup = false,
  };
  esp_err_t err = i2c_new_master_bus(&i2c_bus_cfg, &global_i2c_handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "I2C new master bus error: 0x%X :: %d", err, err);
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