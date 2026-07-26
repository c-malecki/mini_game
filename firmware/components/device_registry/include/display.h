#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#include "freertos/FreeRTOS.h"
#include "esp_lcd_io_i2c.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_sh1106.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "font.h"
#include "freertos/FreeRTOS.h"
#include <string.h>
#include <stdbool.h>

#define DISPLAY_I2C_BUS I2C_NUM_0
#define DISPLAY_I2C_ADDR (0x3C)

#define DISPLAY_SDA_PIN (22)
#define DISPLAY_SCL_PIN (23)

#define DISPLAY_WIDTH (128)
#define DISPLAY_HEIGHT (64)

typedef enum {
    DISPLAY_CMD_NONE = 0,
    DISPLAY_CMD_CLEAR,
    DISPLAY_CMD_FILL,
    DISPLAY_CMD_FLUSH,
    //
    DISPLAY_CMD_DRAW_SPRITE,
    DISPLAY_CMD_DRAW_LINE,
    DISPLAY_CMD_DRAW_RECTANGLE,
    DISPLAY_CMD_DRAW_CIRCLE,
    DISPLAY_CMD_DRAW_TEXT,
    DISPLAY_CMD_DRAW_FLOAT,
    //
    DISPLAY_CMD_CLEAR_RECTANGLE,
    DISPLAY_CMD_CLEAR_FLOAT,
} Display_CMDs;

typedef struct
{
    Display_CMDs CMD;
    union {
        int x0;
        int y0;
        int x1;
        int y1;
        bool on;
    } pline;
    union {
        int x;
        int y;
        int w;
        int hgt;
        bool fill;
    } prect;
    union {
        int xc;
        int yc;
        int r;
        bool fill;
    } pcirc;
    union {
        int x;
        int y;
        char c;
        bool on;
    } pchar;
    union {
        int x;
        int y;
        const char *text;
        bool on;
    } ptext;
    union {
        int x;
        int y;
        float value;
        int decimals;
        bool on;
    } pfloat;
} Display_CMD_Param_t;

typedef struct
{
    Display_CMD_Param_t pending_cmd;
} Display_t;

void Display_Init(void);

Display_CMD_Param_t Display_GetPendingCMD(void);
void Display_SetPendingCMD(Display_CMD_Param_t cmd);

//

void Display_Clear(void);
void Display_Fill(void);
void Display_Flush(void);

void Display_DrawPixel(int x, int y, bool on);

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

#endif // __DISPLAY_H_