#include "game_engine.h"
#include "controls.h"
#include "display.h"
#include "tracks.h"
#include <string.h>

void Game_Engine_Init(Game_Engine_t *ge) {
  Display_Init();
  Sound_Init();
  Controls_Init(&ge->controls);
}

void Game_Update(Player_t *player, Controls_t *controls) {
  if (!controls->is_moving)
    return;

  switch (controls->last_direction) {
  case CONTROLS_DIRECTION_UP:
    player->y -= player->speed;
    break;
  case CONTROLS_DIRECTION_DOWN:
    player->y += player->speed;
    break;
  case CONTROLS_DIRECTION_LEFT:
    player->x -= player->speed;
    break;
  case CONTROLS_DIRECTION_RIGHT:
    player->x += player->speed;
    break;
  }

  // clamp to screen bounds
  if (player->x < 0)
    player->x = 0;
  if (player->y < 0)
    player->y = 0;
  if (player->x >= DISPLAY_WIDTH)
    player->x = DISPLAY_WIDTH - 1;
  if (player->y >= DISPLAY_HEIGHT)
    player->y = DISPLAY_HEIGHT - 1;
}

void Game_Render(Player_t *player) {
  Display_DrawRect(player->x, player->y, 4, 4, true);
  Display_Flush();
}

void Game_Engine_PlayTrack(const Track_t *track) { Sound_PlayTrack(track); }