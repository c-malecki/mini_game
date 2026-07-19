#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "controls.h"
#include "sound.h"

typedef struct
{
    int x, y; // player position in pixels
    int speed; // pixels per tick
} Player_t;

typedef struct
{
    Player_t *player;
    Controls_t controls;
} Game_Engine_t;

void Game_Engine_Init(Game_Engine_t *ge);
void Game_Update(Player_t *player, Controls_t *controls);
void Game_Render(Player_t *player);
void Game_Engine_PlayTrack(const Track_t *track);

#endif // __PLAYER_H_