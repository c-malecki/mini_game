#include "sound.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <stdlib.h>

// Global tracking references
static const Track_t *current_track = NULL;
static TaskHandle_t sound_task_handle = NULL;
static uint32_t current_bpm = 160;
static volatile bool sfx_pending;
static int32_t master_volume;

// Track the state of the active music note so SFX can pause/resume cleanly
static int32_t active_music_freq = NOTE_REST;
static uint32_t active_note_remaining_ms = 0;

static void sound_engine_task(void *arg);

void Sound_Init() {
  ledc_timer_config_t timer_cfg = {.speed_mode = SOUND_DRIVER_MODE,
                                   .timer_num = SOUND_DRIVER_TIMER,
                                   .duty_resolution = SOUND_DRIVER_RESOLUTION,
                                   .freq_hz = NOTE_A4,
                                   .clk_cfg = LEDC_AUTO_CLK};
  ledc_timer_config(&timer_cfg);

  ledc_channel_config_t channel_cfg = {.speed_mode = SOUND_DRIVER_MODE,
                                       .channel = SOUND_DRIVER_CHANNEL,
                                       .timer_sel = SOUND_DRIVER_TIMER,
                                       .gpio_num = SOUND_DRIVER_PWM_PIN,
                                       .duty = SOUND_DRIVER_DUTY_OFF,
                                       .hpoint = 0};
  ledc_channel_config(&channel_cfg);

  xTaskCreate(sound_engine_task, "sound_engine", 3072, NULL, 5,
              &sound_task_handle);
}

void Sound_PlayTrack(const Track_t *track) {
  current_track = track;
  active_note_remaining_ms = 0;
}

void Sound_SetTempo(uint32_t bpm) {
  if (bpm > 0) {
    current_bpm = bpm;
  }
}

// Thread-safe function to trigger your laser blast sound effect from game code
void Sound_TriggerSFX() {
  sfx_pending = true;
  // Instantly force the sound engine task to wake up from vTaskDelay
  // to handle the sound effect without waiting for the current musical note
  // to finish!
  if (sound_task_handle != NULL) {
    xTaskNotifyGive(sound_task_handle);
  }
}

uint32_t calculate_note_ms(Note_Duration_t duration) {
  if (duration == DURATION_END)
    return 0;
  // Base tempo calculation: 60000ms / BPM = 1/4 note length
  uint32_t quarter_note_ms = 60000 / current_bpm;

  // Scale duration fractions proportionally
  return (quarter_note_ms * 4) / (uint32_t)duration;
}

static void apply_hardware_tone(int32_t frequency) {
  if (frequency <= NOTE_REST || frequency > 10000) {
    ledc_set_duty(SOUND_DRIVER_MODE, SOUND_DRIVER_CHANNEL,
                  SOUND_DRIVER_DUTY_OFF);
    ledc_update_duty(SOUND_DRIVER_MODE, SOUND_DRIVER_CHANNEL);
  } else {
    ledc_set_freq(SOUND_DRIVER_MODE, SOUND_DRIVER_TIMER, frequency);
    ledc_set_duty(SOUND_DRIVER_MODE, SOUND_DRIVER_CHANNEL,
                  SOUND_DRIVER_DUTY_ON);
    ledc_update_duty(SOUND_DRIVER_MODE, SOUND_DRIVER_CHANNEL);
  }
}

static void sound_engine_task(void *arg) {
  size_t phrase_idx = 0;
  int note_idx = 0;
  int32_t sfx_step = 0;
  const Note_t *current_phrase = NULL;

  // Small slicing step (ms) for high resolution mixer polling
  const uint32_t engine_tick_ms = 10;

  while (1) {
    // ─── PART 1: PRIORITY SOUND EFFECT HANDLER ───
    if (sfx_pending) {
      // Retro Laser Blast Procedural Curve: 1800Hz down to 200Hz
      int32_t sfx_freq = 1800 - (sfx_step * 60);
      sfx_step++;

      if (sfx_freq <= 200) {
        sfx_pending = false; // SFX finished processing
        sfx_step = 0;
        apply_hardware_tone(NOTE_REST); // Brief reset tap
      } else {
        apply_hardware_tone(sfx_freq);
      }

      // Sleep for the resolution of the SFX step sequence
      vTaskDelay(pdMS_TO_TICKS(engine_tick_ms));
      continue; // Bypass music track tick to grant SFX total priority
    }

    // ─── PART 2: MUSIC PHRASE SEQUENCER (TICK-BASED) ───
    if (current_track != NULL) {
      // If the last note finished playing, load up the next note in the struct
      // structure
      if (active_note_remaining_ms == 0) {

        // Track initialization or bounds rolling check
        if (current_phrase == NULL) {
          phrase_idx = 0;
          note_idx = 0;
          uint8_t pid = current_track->order[phrase_idx];
          current_phrase = current_track->phrases[pid];
        }

        if (current_phrase[note_idx].duration_ms == DURATION_END) {
          phrase_idx++;

          if (phrase_idx >= current_track->order_len) {
            // Song ended! Infinite loop reset or close track
            current_track = NULL;
            current_phrase = NULL;
            apply_hardware_tone(NOTE_REST);
            continue;
          }

          // Load the next valid phrase mapped out in the order table
          uint8_t pid = current_track->order[phrase_idx];
          current_phrase = current_track->phrases[pid];
          note_idx = 0;
        }

        // Process and buffer the selected note attributes
        Note_t note = current_phrase[note_idx];
        active_music_freq = note.frequency;
        active_note_remaining_ms = calculate_note_ms(note.duration_ms);

        note_idx++; // Increment counter ready for next step iteration

        // Apply crisp note separation gap if not a rest
        if (active_music_freq != NOTE_REST) {
          apply_hardware_tone(NOTE_REST);
          vTaskDelay(pdMS_TO_TICKS(15));
        }
      }

      // Play buffered active music note
      apply_hardware_tone(active_music_freq);

      // Decrement the note tracking timer down by the engine frame-slice value
      // We use ulTaskNotifyTake to wait up to engine_tick_ms.
      // If a Sound Effect fires, it cancels this delay instantly to service the
      // laser!
      uint32_t awakened =
          ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(engine_tick_ms));

      if (awakened) {
        // Instantly broken out by an SFX trigger; don't decrement music
        // countdown yet
        continue;
      }

      if (active_note_remaining_ms >= engine_tick_ms) {
        active_note_remaining_ms -= engine_tick_ms;
      } else {
        active_note_remaining_ms = 0;
      }

    } else {
      // Idle Loop when no song is actively playing
      apply_hardware_tone(NOTE_REST);
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}
