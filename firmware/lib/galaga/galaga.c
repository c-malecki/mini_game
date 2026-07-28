#include "galaga.h"
#include "game.h"

static void controls_pause_game(void) { GEng_SND_TogglePauseTrack(); }

static void controls_basic_attack(void) { GEng_SND_PlaySfx(); }

static void controls_special_attack(void) {}

static void controls_move_up(void) {
  Unit_t player;
  GEng_GAME_GetPlayer(&player);
  int new_x = player.x;
  int new_y = player.y;
  new_y -= player.spd;
  GEng_GAME_SetPlayerPos(new_x, new_y);
}

static void controls_move_down(void) {
  Unit_t player;
  GEng_GAME_GetPlayer(&player);
  int new_x = player.x;
  int new_y = player.y;
  new_y += player.spd;
  GEng_GAME_SetPlayerPos(new_x, new_y);
}

static void controls_move_left(void) {
  Unit_t player;
  GEng_GAME_GetPlayer(&player);
  int new_x = player.x;
  int new_y = player.y;
  new_x -= player.spd;
  GEng_GAME_SetPlayerPos(new_x, new_y);
}

static void controls_move_right(void) {
  Unit_t player;
  GEng_GAME_GetPlayer(&player);
  int new_x = player.x;
  int new_y = player.y;
  new_x += player.spd;
  GEng_GAME_SetPlayerPos(new_x, new_y);
}

const Unit_t player = {.x = 0,
                       .y = 32,
                       .spd = 3,
                       .hp = 3,
                       .sp = 3,
                       .dmg = 1,
                       .sprite = &SPRITE_PLAYER_SHIP};

const Unit_t enemy1 = {.x = 110,
                       .y = 32,
                       .spd = 2,
                       .hp = 1,
                       .sp = 1,
                       .dmg = 1,
                       .sprite = &SPRITE_PLAYER_SHIP};

Game_t Galaga = {.controls =
                     {
                         .on_press_cmd = controls_pause_game,
                         .on_press_a = controls_basic_attack,
                         .on_press_b = controls_special_attack,
                         .on_press_up = controls_move_up,
                         .on_press_down = controls_move_down,
                         .on_press_left = controls_move_left,
                         .on_press_right = controls_move_right,
                     },
                 .player = player,
                 .enemies = {enemy1}};
