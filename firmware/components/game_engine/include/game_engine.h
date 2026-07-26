#ifndef __GAME_ENGINE_H_
#define __GAME_ENGINE_H_

#include "controls.h"
#include "player.h"
#include "sprite.h"
#include "device_registry.h"
#include "esp_log.h"

typedef struct
{
    Player_t player;
    Controls_t controls;
} Game_Engine_t;

/************ GAME ENGINE **************/

void GEng_Init(Player_t player);

void GEng_LOOP_HandleInput(void);
void GEng_LOOP_Render(void);

/*
    GEnd_Game_Load
    - set player shape/speed/etc...
    - initial screen and everything etc...
*/

/************ DISPLAY **************/

void GEng_DSP_Fill(void);
void GEng_DSP_Clear(void);
void GEng_DSP_DrawSprite(int x, int y, const Sprite_t *sprite, bool on);

/************ SOUND **************/

void GEng_SND_PlayTrack(const Track_t *track);
void GEng_SND_TogglePauseTrack(void);
void GEng_SND_PlaySfx(void);

/************ GAME **************/

void GEng_GAME_SetPlayer(Player_t player);

/************ DEBUG **************/

void GEng_PrintState(void);

#endif // __GAME_ENGINE_H_