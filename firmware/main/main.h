#ifndef __MAIN_H_
#define __MAIN_H_

#include "device_system.h"
#include "game_engine.h"

#define DEVICE_NAME "GAME_TEST"

typedef struct
{
    Game_Engine_t game_engine;
    Device_System_t device_system;
} App_t;

#endif // __MAIN_H_