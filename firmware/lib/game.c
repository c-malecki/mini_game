#include "game.h"
#include <string.h>

void Game_Update(Player_t *player, Controller_t *controller) {
  if (!controller->is_moving)
    return;

  switch (controller->last_direction) {
  case CONTROLLER_DIRECTION_UP:
    player->y -= player->speed;
    break;
  case CONTROLLER_DIRECTION_DOWN:
    player->y += player->speed;
    break;
  case CONTROLLER_DIRECTION_LEFT:
    player->x -= player->speed;
    break;
  case CONTROLLER_DIRECTION_RIGHT:
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

void Game_Render(Display_t *display, Player_t *player) {
  memset(display->buffer, 0, sizeof(display->buffer));
  Display_DrawRect(display, player->x, player->y, 4, 4, true);
  Display_Flush(display);
}