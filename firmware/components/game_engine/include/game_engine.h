#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "controls.h"
#include "sound.h"

typedef struct
{
    int x;
    int y;
    int speed; // pixels per tick
} Player_t;

typedef struct
{
    Player_t *player;
    Controls_t controls;
} Game_Engine_t;

void GEng_Init(void);
void GEng_HandleInput(void);

void GEng_Display_DrawLine(int x0, int y0, int x1, int y1, bool on);
void GEng_Display_DrawRect(int x, int y, int w, int hgt, bool fill);
void GEng_ClearRect(int x, int y, int w, int hgt);
void GEng_Display_DrawCirc(int xc, int yc, int r, bool fill);
void GEng_Display_DrawChar(int x, int y, char c, bool on);
void GEng_Display_DrawText(int x, int y, const char *text, bool on);

void GEng_Display_DrawFloat(int x, int y, float value, int decimals, bool on);
void GEng_Display_ClearFloat(int x, int y, float value, int decimals);

//

void GEng_Display_Flush(void);
void GEnd_Dsp_Clear(void);
void GEng_Display_Render(void);
void GEng_CleanUp(void);

void GEng_Sound_PlayTrack(const Track_t *track);
void GEng_Sound_TogglePauseTrack(void);
void GEng_Sound_PlaySfx(void);

#endif // __PLAYER_H_