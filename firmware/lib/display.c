#include "display.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "ssd1306.h"

void Display_Init(Display_t *display, i2c_master_bus_handle_t i2c_bus_handle) {
  ssd1306_config_t cfg = {
      .bus = SSD1306_I2C,
      .width = 128,
      .height = 64,
      .iface.i2c =
          {
              .port = I2C_NUM_0,
              .addr = DISPLAY_I2C_ADDR,
          },
  };

  ssd1306_handle_t handle;
  // ESP_ERROR_CHECK(ssd1306_new_i2c(&cfg, &handle));

  esp_err_t err = ssd1306_new_i2c(&cfg, &handle);
  if (err != ESP_OK) {
    ESP_LOGE("GAME_TEST", "Display init error: 0x%X :: %d", err, err);
  }

  ssd1306_clear(handle);
  ssd1306_draw_text(handle, 0, 0, "Game Test", true);
  ssd1306_display(handle);
  display->handle = handle;
}

void Display_DrawCirc(Display_t *display, int xc, int yc, int r, bool fill) {
  ;
}
void Display_DrawLine(Display_t *display, int x0, int y0, int x1, int y1,
                      bool on) {
  ;
}
void Display_DrawRect(Display_t *display, int x, int y, int w, int hgt,
                      bool fill) {
  ;
}
void Display_DrawPixl(Display_t *display, int x, int y, bool on) { ; }
void Display_DrawTxt(Display_t *display, int x, int y, const char *text,
                     bool on) {
  ;
}
void Display_DrawTxtS(Display_t *display, int x, int y, const char *text,
                      bool on, int scale) {
  ;
}
void Display_DrawTxtWS(Display_t *display, int x, int y, int w, int hgt,
                       const char *text, bool on, int scale) {
  ;
}
