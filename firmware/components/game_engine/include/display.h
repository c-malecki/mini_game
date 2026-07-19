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

void Display_Init(void);

void Display_DrawPixl(int x, int y, bool on);

void Display_DrawLine(int x0, int y0, int x1, int y1, bool on);
void Display_ClearLine(int x0, int y0, int x1, int y1);

void Display_DrawRect(int x, int y, int w, int hgt, bool fill);
void Display_ClearRect(int x, int y, int w, int hgt);

void Display_DrawCirc(int xc, int yc, int r, bool fill);
void Display_ClearCirc(int xc, int yc, int r, bool fill);

void Display_DrawChar(int x, int y, char c, bool on);
void Display_ClearChar(int x, int y, char c);

void Display_DrawText(int x, int y, const char *text, bool on);
void Display_ClearText(int x, int y, const char *text);

void Display_DrawFloat(int x, int y, float value, int decimals, bool on);
void Display_ClearFloat(int x, int y, float value, int decimals);

void Display_Flush(void);

#endif // __DISPLAY_H_