#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "display.h"
#include "controller_driver.h"

typedef struct
{
    int x, y; // player position in pixels
    int speed; // pixels per tick
} Player_t;

void Game_Update(Player_t *player, Controller_t *controller);
void Game_Render(Display_t *display, Player_t *player);

#endif // __PLAYER_H_