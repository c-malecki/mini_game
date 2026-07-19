#include "game_engine.h"
#include "controls.h"
#include "display.h"
#include "sound.h"
#include "tracks.h"

static Game_Engine_t game_engine;

void GEng_Init() {
  Player_t player = {.x = 64, .y = 32, .speed = 2};
  game_engine.player = &player;
  Display_Init();
  Sound_Init();
  Controls_Init(&game_engine.controls);
}

void GEng_Display_DrawLine(int x0, int y0, int x1, int y1, bool on) {
  Display_DrawLine(x0, y0, x1, y1, on);
};

void GEng_Display_DrawRect(int x, int y, int w, int hgt, bool fill) {
  Display_DrawRect(x, y, w, hgt, fill);
};

void GEng_ClearRect(int x, int y, int w, int hgt) {
  Display_ClearRect(x, y, w, hgt);
};

void GEng_Display_DrawCirc(int xc, int yc, int r, bool fill) {
  Display_DrawCirc(xc, yc, r, fill);
};

void GEng_Display_DrawChar(int x, int y, char c, bool on) {
  Display_DrawChar(x, y, c, on);
};

void GEng_Display_DrawText(int x, int y, const char *text, bool on) {
  Display_DrawText(x, y, text, on);
};

void GEng_Display_DrawFloat(int x, int y, float value, int decimals, bool on) {
  Display_DrawFloat(x, y, value, decimals, on);
};

void GEng_Display_ClearFloat(int x, int y, float value, int decimals) {
  Display_ClearFloat(x, y, value, decimals);
};

// ************Display Exec************

void GEng_Display_Flush() { Display_Flush(); };

void GEnd_Dsp_Clear(void) {
  GEng_ClearRect(0, 0, 64, 128);
  GEng_Display_Flush();
}

void GEng_Display_Render() {
  GEng_Display_DrawRect(game_engine.player->x, game_engine.player->y, 4, 4,
                        true);
  GEng_Display_Flush();
}

void GEng_CleanUp(void) { Controls_CleanUp(); }

// ***********SOUND*************

void GEng_Sound_PlayTrack(const Track_t *track) { Sound_PlayTrack(track); }

void GEng_Sound_TogglePauseTrack(void) { Sound_TogglePauseTrack(); }

void GEng_Sound_PlaySfx(void) { Sound_TriggerSFX(); }

// ************Game Engine************

void GEng_HandleInput(void) {
  Controls_Directions dir = Controls_GetPendingDirection();
  Controls_Buttons btn = Controls_GetPendingBtn();

  switch (dir) {
  case CONTROLS_DIRECTION_UP:
    game_engine.player->y -= game_engine.player->speed;
    break;
  case CONTROLS_DIRECTION_DOWN:
    game_engine.player->y += game_engine.player->speed;
    break;
  case CONTROLS_DIRECTION_LEFT:
    game_engine.player->x -= game_engine.player->speed;
    break;
  case CONTROLS_DIRECTION_RIGHT:
    game_engine.player->x += game_engine.player->speed;
    break;
  default:
    break;
  }

  switch (btn) {
  case CONTROLS_BUTTON_CMD:
    GEng_Sound_TogglePauseTrack();
    break;
  case CONTROLS_BUTTON_A:
    GEng_Sound_PlaySfx();
    break;
  default:
    break;
  }

  // clamp to screen bounds
  if (game_engine.player->x < 0)
    game_engine.player->x = 0;
  if (game_engine.player->y < 0)
    game_engine.player->y = 0;
  if (game_engine.player->x >= DISPLAY_WIDTH)
    game_engine.player->x = DISPLAY_WIDTH - 1;
  if (game_engine.player->y >= DISPLAY_HEIGHT)
    game_engine.player->y = DISPLAY_HEIGHT - 1;
}