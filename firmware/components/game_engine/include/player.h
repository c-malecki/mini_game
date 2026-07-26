#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "sprite.h"

typedef struct
{
    int x;
    int y;
    int speed; // pixels per tick
    const Sprite_t *sprite;
} Player_t;

#endif // __PLAYER_H_