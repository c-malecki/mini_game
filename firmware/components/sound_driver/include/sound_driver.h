#ifndef __SOUND_DRIVER_H_
#define __SOUND_DRIVER_H_

#include <stdint.h>
#include "freertos/idf_additions.h"

#define SOUND_DRIVER_PWM_PIN 21
#define SOUND_DRIVER_TIMER LEDC_TIMER_0
#define SOUND_DRIVER_MODE LEDC_LOW_SPEED_MODE
#define SOUND_DRIVER_CHANNEL LEDC_CHANNEL_0
// 0-1023 duty steps
#define SOUND_DRIVER_RESOLUTION LEDC_TIMER_10_BIT

#define SOUND_DRIVER_DUTY_OFF (0)
#define SOUND_DRIVER_DUTY_ON (512)

//
#define NOTE_REST 0
#define NOTE_END -1
#define NOTE_BASE_WHOLE_MS 1200

// Octave 1
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62

// Octave 2
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123

// Octave 3
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247

// Octave 4
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494

// Octave 5
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988

// Octave 6
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976

// Octave 7
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951

// Octave 8
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978
#define NOTE_E8 5274
#define NOTE_F8 5588
#define NOTE_FS8 5920
#define NOTE_G8 6272
#define NOTE_GS8 6645
#define NOTE_A8 7040
#define NOTE_AS8 7459
#define NOTE_B8 7902

typedef enum {
    DURATION_WHOLE = 1, // 4 beats (1/1)
    DURATION_HALF = 2, // 2 beats (1/2)
    DURATION_QUARTER = 4, // 1 beat  (1/4)
    DURATION_EIGHTH = 8, // 0.5 beats (1/8)
    DURATION_SIXTEENTH = 16, // 0.25 beats (1/16)
    DURATION_END = 0 // End marker for phrases
} Note_Duration_t;

typedef struct
{
    int frequency;
    Note_Duration_t duration_ms;
} Note_t;

typedef struct
{
    const uint8_t *order;
    size_t order_len;
    const Note_t **phrases;
} Track_t;

typedef struct
{
    Track_t *current_track;
    TaskHandle_t sound_task;
} Sound_t;

void Sound_Init(Sound_t *mp);
void Sound_PlayTrack(const Track_t *track);
void Sound_SetTempo(uint32_t bpm);

#endif // __SOUND_DRIVER_H_