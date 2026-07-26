#ifndef __GAME_H_
#define __GAME_H_

#include <stdint.h>

typedef struct
{
    const uint8_t *data;
    uint8_t width;
    uint8_t height;
} Sprite_t;

typedef struct
{
    int x; // 0
    int y; // 32
    int hp; // 3
    int sp; // 3
    int spd; // 3
    int dmg; // 1
    const Sprite_t *sprite; // SPRITE_PLAYER_SHIP
} Unit_t;

typedef struct
{

} Game_Controls_t;

typedef struct
{
    const char title;
    Game_Controls_t controls;
    Unit_t player;
    Unit_t enemies[];
} Game_t;

#endif // __GAME_H_