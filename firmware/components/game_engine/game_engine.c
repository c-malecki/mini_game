#include "game_engine.h"
#include "display.h"
#include "esp_log.h"
#include "game.h"

static Game_Engine_t game_engine;

/************ GAME ENGINE **************/

void GEng_Init(Game_t *game) {
  game_engine.game = game;
  DReg_Init();
  Controls_Init(&game->controls);
}

void GEng_LOOP_HandleInput(void) {
  /* buttons */

  if (Controls_IsPinActive(CONTROLS_PIN_CMD)) {
    if (game_engine.game->controls.on_press_cmd != NULL) {
      game_engine.game->controls.on_press_cmd();
    }
  }

  // int64_t cmd_held = Controls_GetPinActiveMS(CONTROLS_BTN_CMD);
  // if (cmd_held >= 3000) { TODO: toggle deep sleep }

  if (Controls_IsPinActive(CONTROLS_PIN_A)) {
    if (game_engine.game->controls.on_press_cmd != NULL) {
      game_engine.game->controls.on_press_a();
    }
  }

  // int64_t a_held = Controls_GetPinActiveMS(CONTROLS_BTN_A);
  // if (a_held >= 3000) { }

  if (Controls_IsPinActive(CONTROLS_PIN_B)) {
    if (game_engine.game->controls.on_press_cmd != NULL) {
      game_engine.game->controls.on_press_b();
    }
  }

  // int64_t b_held = Controls_GetPinActiveMS(CONTROLS_BTN_B);
  // if (b_held >= 3000) { }

  /* movement/direction */

  if (Controls_IsPinActive(CONTROLS_PIN_UP)) {
    if (game_engine.game->controls.on_press_cmd != NULL) {
      game_engine.game->controls.on_press_up();
    }
  }

  if (Controls_IsPinActive(CONTROLS_PIN_DOWN)) {
    if (game_engine.game->controls.on_press_cmd != NULL) {
      game_engine.game->controls.on_press_down();
    }
  }

  if (Controls_IsPinActive(CONTROLS_PIN_LEFT)) {
    if (game_engine.game->controls.on_press_cmd != NULL) {
      game_engine.game->controls.on_press_left();
    }
  }

  if (Controls_IsPinActive(CONTROLS_PIN_RIGHT)) {
    if (game_engine.game->controls.on_press_cmd != NULL) {
      game_engine.game->controls.on_press_right();
    }
  }

  if (game_engine.game->player.x < 0) {
    game_engine.game->player.x = 0;
  }

  if (game_engine.game->player.y < 0) {
    game_engine.game->player.y = 0;
  }

  if (game_engine.game->player.x >
      DISPLAY_WIDTH - game_engine.game->player.sprite->width) {
    game_engine.game->player.x =
        DISPLAY_WIDTH - game_engine.game->player.sprite->width;
  }

  if (game_engine.game->player.y >=
      DISPLAY_HEIGHT - game_engine.game->player.sprite->height) {
    game_engine.game->player.y =
        DISPLAY_HEIGHT - game_engine.game->player.sprite->height;
  }
}

void GEng_LOOP_Render() {
  static int last_x = 0;
  static int last_y = 0;

  Display_ClearRect(last_x, last_y, game_engine.game->player.sprite->width,
                    game_engine.game->player.sprite->height);

  GEng_DSP_DrawSprite(game_engine.game->player.x, game_engine.game->player.y,
                      game_engine.game->player.sprite, true);

  GEng_DSP_DrawSprite(game_engine.game->enemies[0].x,
                      game_engine.game->enemies[0].y,
                      game_engine.game->enemies[0].sprite, true);

  Display_Flush();

  last_x = game_engine.game->player.x;
  last_y = game_engine.game->player.y;
}

/************ GAME **************/

void GEng_GAME_GetPlayer(Unit_t *player) { *player = game_engine.game->player; }

void GEng_GAME_SetPlayerPos(int x, int y) {
  game_engine.game->player.x = x;
  game_engine.game->player.y = y;
};

/************ DISPLAY **************/

void GEng_DSP_Fill(void) {
  Display_DrawRect(0, 0, 128, 64, true);
  Display_Flush();
}

void GEng_DSP_Clear(void) {
  Display_ClearRect(0, 0, 128, 64);
  Display_Flush();
}

void GEng_DSP_DrawSprite(int x, int y, const Sprite_t *sprite, bool on) {
  int stride = (sprite->width + 7) / 8;
  for (int row = 0; row < sprite->height; row++) {
    for (int col = 0; col < sprite->width; col++) {
      uint8_t byte = sprite->data[row * stride + col / 8];
      if ((byte >> (7 - (col % 8))) & 1) {
        Display_DrawPixel(x + col, y + row, on);
      }
    }
  }
}

/************ SOUND **************/

void GEng_SND_PlayTrack(const Track_t *track) { Sound_PlayTrack(track); }

void GEng_SND_TogglePauseTrack(void) { Sound_TogglePauseTrack(); }

void GEng_SND_PlaySfx(void) { Sound_TriggerSFX(); }
