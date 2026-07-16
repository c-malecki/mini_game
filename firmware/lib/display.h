#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#include "freertos/FreeRTOS.h"
#include "esp_lcd_io_i2c.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_sh1106.h"
#include "driver/i2c_master.h"

#define DISPLAY_I2C_BUS I2C_NUM_0
#define DISPLAY_I2C_ADDR (0x3C)

#define DISPLAY_SDA_PIN (22)
#define DISPLAY_SCL_PIN (23)

#define DISPLAY_WIDTH (128)
#define DISPLAY_HEIGHT (64)

typedef struct
{
    esp_lcd_panel_handle_t panel;
    uint8_t buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];
} Display_t;

void Display_Init(Display_t *display);
void Display_Test(Display_t *display);

void Display_DrawPixl(Display_t *display, int x, int y, bool on);
void Display_DrawLine(Display_t *display, int x0, int y0, int x1, int y1, bool on);
void Display_DrawRect(Display_t *display, int x, int y, int w, int hgt, bool fill);
void Display_ClearRect(Display_t *display, int x, int y, int w, int hgt);
void Display_DrawCirc(Display_t *display, int xc, int yc, int r, bool fill);
void Display_DrawText(Display_t *display, int x, int y, const char *text, bool on);
void Display_DrawChar(Display_t *display, int x, int y, char c, bool on);
void Display_DrawFloat(Display_t *display, int x, int y, float value, int decimals, bool on);
void Display_Flush(Display_t *display);

#endif // __DISPLAY_H_