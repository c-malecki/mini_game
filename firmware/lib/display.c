#include "display.h"
#include "driver/i2c_master.h"
#include "esp_lcd_io_i2c.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_sh1106.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include <string.h>

static i2c_master_bus_handle_t i2c_bus_handle = NULL;

void Display_Init(Display_t *display) {
  i2c_master_bus_config_t i2c_bus_cfg = {
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .i2c_port = I2C_NUM_0,
      .sda_io_num = DISPLAY_SDA_PIN,
      .scl_io_num = DISPLAY_SCL_PIN,
      .glitch_ignore_cnt = 7,
      .intr_priority = 0,
      .trans_queue_depth = 0,
      .flags =
          {
              .enable_internal_pullup = false,
              .allow_pd = false,
          },
  };
  esp_err_t err = i2c_new_master_bus(&i2c_bus_cfg, &i2c_bus_handle);
  if (err != ESP_OK) {
    ESP_LOGE("DISPLAY", "I2C new master bus error: 0x%X :: %d", err, err);
  }

  // Create the i2c io handle
  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_i2c_config_t io_config = ESP_SH1106_DEFAULT_IO_CONFIG;
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_io_i2c(i2c_bus_handle, &io_config, &io_handle));

  /* SCREEN CONFIGURATION */

  // sh1106 panel configuration (most of the values are not used, but must be
  // set to avoid cpp warnings)
  esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = -1, // sh1106 does not have a reset pin, so set to -1
      .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB, // not even used, but must be
                                                  // set to avoid cpp warnings
      .data_endian = LCD_RGB_DATA_ENDIAN_LITTLE,  // not even used, but must be
                                                  // set to avoid cpp warnings
      .bits_per_pixel =
          SH1106_PIXELS_PER_BYTE / 8, // bpp = 1 (monochrome, that's important)
      .flags =
          {
              .reset_active_high =
                  false, // not even used, but must be set to avoid cpp warnings
          },
      .vendor_config =
          NULL, // no need for custom vendor config, not implemented
  };

  // Create the panel handle from the sh1106 driver
  esp_lcd_panel_handle_t panel_handle = NULL;
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_sh1106(io_handle, &panel_config, &panel_handle));
  display->panel = panel_handle;

  // Reset the screen (no reset pin, so it's a no-op here, optional)
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));

  // Initialize the screen (this one isn't optional at all!)
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

  // Turn on the screen (Easier to see something, right?)
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
}

void Display_Test(Display_t *display) {
  uint8_t buffer_data[SH1106_BUFFER_SIZE];
  memset(buffer_data, 0, SH1106_BUFFER_SIZE);

  buffer_data[0] = 0b00000001;
  buffer_data[SH1106_BUFFER_SIZE - 1] = 0b10000000;

  ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(display->panel, 0, 0, SH1106_WIDTH,
                                            SH1106_HEIGHT, buffer_data));
  vTaskDelay(pdMS_TO_TICKS(2000));

  memset(buffer_data, 0xFF, SH1106_BUFFER_SIZE);
  ESP_ERROR_CHECK(
      esp_lcd_panel_draw_bitmap(display->panel, 48, 16, 80, 48, buffer_data));
  vTaskDelay(pdMS_TO_TICKS(2000));

  memset(buffer_data, 0x00, SH1106_BUFFER_SIZE);
  memset(buffer_data, 0xFF, 8);
  for (int y = 0; y < SH1106_HEIGHT; y += 8) {
    for (int x = 0; x < SH1106_WIDTH; x += 8) {
      ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(display->panel, x, y, x + 8,
                                                y + 8, buffer_data));
      vTaskDelay(pdMS_TO_TICKS(50));
    }
  }
}

void Display_DrawPixl(Display_t *display, int x, int y, bool on) {
  if (x < 0 || x >= DISPLAY_WIDTH || y < 0 || y >= DISPLAY_HEIGHT)
    return;

  int index = (y / 8) * DISPLAY_WIDTH + x;
  uint8_t bit = 1 << (y % 8);

  if (on) {
    display->buffer[index] |= bit;
  } else {
    display->buffer[index] &= ~bit;
  }
}

void Display_DrawLine(Display_t *display, int x0, int y0, int x1, int y1,
                      bool on) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;

  while (1) {
    Display_DrawPixl(display, x0, y0, on);
    if (x0 == x1 && y0 == y1)
      break;
    int e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
  }
}

void Display_DrawRect(Display_t *display, int x, int y, int w, int hgt,
                      bool fill) {
  if (fill) {
    for (int j = y; j < y + hgt; j++) {
      Display_DrawLine(display, x, j, x + w - 1, j, true);
    }
  } else {
    Display_DrawLine(display, x, y, x + w - 1, y, true);
    Display_DrawLine(display, x, y + hgt - 1, x + w - 1, y + hgt - 1, true);
    Display_DrawLine(display, x, y, x, y + hgt - 1, true);
    Display_DrawLine(display, x + w - 1, y, x + w - 1, y + hgt - 1, true);
  }
}

void Display_DrawCirc(Display_t *display, int xc, int yc, int r, bool fill) {
  int x = r, y = 0, err = 0;

  while (x >= y) {
    if (fill) {
      Display_DrawLine(display, xc - x, yc + y, xc + x, yc + y, true);
      Display_DrawLine(display, xc - x, yc - y, xc + x, yc - y, true);
      Display_DrawLine(display, xc - y, yc + x, xc + y, yc + x, true);
      Display_DrawLine(display, xc - y, yc - x, xc + y, yc - x, true);
    } else {
      Display_DrawPixl(display, xc + x, yc + y, true);
      Display_DrawPixl(display, xc + y, yc + x, true);
      Display_DrawPixl(display, xc - y, yc + x, true);
      Display_DrawPixl(display, xc - x, yc + y, true);
      Display_DrawPixl(display, xc - x, yc - y, true);
      Display_DrawPixl(display, xc - y, yc - x, true);
      Display_DrawPixl(display, xc + y, yc - x, true);
      Display_DrawPixl(display, xc + x, yc - y, true);
    }

    y += 1;
    if (err <= 0) {
      err += 2 * y + 1;
    }
    if (err > 0) {
      x -= 1;
      err -= 2 * x + 1;
    }
  }
}

void Display_Flush(Display_t *display) {
  ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(display->panel, 0, 0, DISPLAY_WIDTH,
                                            DISPLAY_HEIGHT, display->buffer));
}

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
