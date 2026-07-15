#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#define DISPLAY_SDA_PIN (22)
#define DISPLAY_SCL_PIN (23)
#define DISPLAY_I2C_ADDR (0x3C)

#include "ssd1306.h"
// #include "ssd1306.h"

typedef struct
{
    ssd1306_handle_t handle;
} Display_t;

void Display_Init(Display_t *display, i2c_master_bus_handle_t i2c_bus_handle);

void Display_DrawCirc(Display_t *display, int xc, int yc, int r, bool fill);
void Display_DrawLine(Display_t *display, int x0, int y0, int x1, int y1, bool on);
void Display_DrawRect(Display_t *display, int x, int y, int w, int hgt, bool fill);
void Display_DrawPixl(Display_t *display, int x, int y, bool on);
void Display_DrawTxt(Display_t *display, int x, int y, const char *text, bool on);
void Display_DrawTxtS(Display_t *display, int x, int y, const char *text, bool on, int scale);
void Display_DrawTxtWS(Display_t *display, int x, int y, int w, int hgt, const char *text, bool on,
                       int scale);

#endif // __DISPLAY_H_