#ifndef __SPRITE_H_
#define __SPRITE_H_

#include <stdint.h>

typedef struct
{
    const uint8_t *data;
    uint8_t width;
    uint8_t height;
} Sprite_t;

#endif // __SPRITE_H_