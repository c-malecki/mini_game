#include "game_engine.h"
#include "display.h"
#include "esp_log.h"

static Game_Engine_t game_engine;

/************ GAME ENGINE **************/

void GEng_Init(Player_t player) {
  game_engine.player = player;
  game_engine.player.x = 64;
  game_engine.player.y = 32;
  game_engine.player.speed = 2;

  DR_Init();
  ESP_LOGI("GAME_ENGINE", "Device Registry initialized");
  Controls_Init();
  ESP_LOGI("GAME_ENGINE", "Controls initialized");
}

void GEng_LOOP_HandleInput(void) {
  if (Controls_IsPinHeld(CONTROLS_LEFT_PIN)) {
    game_engine.player.x -= game_engine.player.speed;
  }
  if (Controls_IsPinHeld(CONTROLS_RIGHT_PIN)) {
    game_engine.player.x += game_engine.player.speed;
  }
  if (Controls_IsPinHeld(CONTROLS_UP_PIN)) {
    game_engine.player.y -= game_engine.player.speed;
  }
  if (Controls_IsPinHeld(CONTROLS_DOWN_PIN)) {
    game_engine.player.y += game_engine.player.speed;
  }

  Controls_Buttons btn = Controls_GetPendingBtn();
  switch (btn) {
  case CONTROLS_BUTTON_CMD:
    GEng_SND_TogglePauseTrack();
    break;
  case CONTROLS_BUTTON_A:
    GEng_SND_PlaySfx();
    break;
  default:
    break;
  }

  // clamp to screen bounds
  if (game_engine.player.x < 0) {
    game_engine.player.x = 0;
  }

  if (game_engine.player.y < 0) {
    game_engine.player.y = 0;
  }

  if (game_engine.player.x >= DISPLAY_WIDTH) {
    game_engine.player.x = DISPLAY_WIDTH - 1;
  }

  if (game_engine.player.y >= DISPLAY_HEIGHT) {
    game_engine.player.y = DISPLAY_HEIGHT - 1;
  }
  GEng_PrintState();
  Controls_CleanUp();
}

void GEng_LOOP_Render() {
  // TODO: change so that position is set by game_engine.player
  static int last_x = 64;
  static int last_y = 32;

  Display_ClearRect(last_x, last_y, game_engine.player.sprite->width,
                    game_engine.player.sprite->height);
  GEng_DSP_DrawSprite(game_engine.player.x, game_engine.player.y,
                      game_engine.player.sprite, true);

  Display_Flush();

  last_x = game_engine.player.x;
  last_y = game_engine.player.y;
}

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

/************ GAME **************/

// set game
void GEng_GAME_SetPlayer(Player_t player) { game_engine.player = player; }

/************ DEBUG **************/

void GEng_PrintState(void) {
  if (game_engine.controls.pending_btn != CONTROLS_BUTTON_NONE) {

    ESP_LOGE("Game Engine", "pending btn: %d\nplayer x: %d\n player y: %d\n",
             game_engine.controls.pending_btn, game_engine.player.x,
             game_engine.player.y);
  }
}