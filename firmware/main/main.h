#ifndef __MAIN_H_
#define __MAIN_H_

#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/semphr.h"
#include "sound_driver.h"
#include "display.h"
#include "battery.h"
#include "controller_driver.h"

#define DEVICE_NAME "GAME_TEST"

typedef struct
{
    SemaphoreHandle_t data_mutex;
    Display_t display;
    MusicPlayer_t music_player;
    Battery_t battery;
    Controller_t controls;
} App_t;

#endif // __MAIN_H_