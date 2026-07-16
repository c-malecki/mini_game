#include "sound_driver.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"

static const Track_t *current_track = NULL;
static TaskHandle_t sound_task_handle = NULL;
static uint32_t current_bpm = 120; // Default tempo setting (120 BPM)

static void sound_task(void *pvParameters);

void Sound_Init(Sound_t *mp) {
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

  xTaskCreate(sound_task,   // Function that runs the loop
              "sound_task", // Name of the text thread
              3072,         // Stack size allocated to this thread
              NULL,         // Parameters to pass (none)
              5, // Priority (Keep it lower than your display/game loops)
              &sound_task_handle // Task handle reference
  );
}

void Sound_PlayTrack(const Track_t *track) { current_track = track; }

void Sound_SetTempo(uint32_t bpm) {
  if (bpm > 0) {
    current_bpm = bpm;
  }
}

uint32_t calculate_note_ms(Note_Duration_t duration) {
  // 60000ms divided by BPM gives the millisecond length of 1 standard Quarter
  // Note (1/4)
  uint32_t quarter_note_ms = 60000 / current_bpm;

  // Scale the millisecond length based on the relative note fraction
  switch (duration) {
  case DURATION_WHOLE:
    return quarter_note_ms * 4;
  case DURATION_HALF:
    return quarter_note_ms * 2;
  case DURATION_QUARTER:
    return quarter_note_ms;
  case DURATION_EIGHTH:
    return quarter_note_ms / 2;
  case DURATION_SIXTEENTH:
    return quarter_note_ms / 4;
  default:
    return 0;
  }
}

void play_note(Note_t note) {
  // Safety Net: Guard against corrupt notes out-of-bounds
  if (note.frequency < 0 || note.frequency > 10000) {
    printf("SOUND_ERROR: Caught corrupted note frequency: %d! Dropping note.\n",
           note.frequency);
    return;
  }

  // Dynamically look up the duration based on the active global BPM
  // variable
  // uint32_t duration_ms = calculate_note_ms(note.duration_ms);
  uint32_t duration_ms = NOTE_BASE_WHOLE_MS / note.duration_ms;

  if (note.frequency == NOTE_REST) {
    ledc_set_duty(SOUND_DRIVER_MODE, SOUND_DRIVER_CHANNEL, 0);
    ledc_update_duty(SOUND_DRIVER_MODE, SOUND_DRIVER_CHANNEL);
  } else {
    ledc_set_freq(SOUND_DRIVER_MODE, SOUND_DRIVER_TIMER, note.frequency);
    ledc_set_duty(SOUND_DRIVER_MODE, SOUND_DRIVER_CHANNEL,
                  SOUND_DRIVER_DUTY_ON);
    ledc_update_duty(SOUND_DRIVER_MODE, SOUND_DRIVER_CHANNEL);
  }

  // Delay using the computed millisecond value
  vTaskDelay(pdMS_TO_TICKS(duration_ms));

  // Mandatory 10ms crisp note separation gap
  ledc_set_duty(SOUND_DRIVER_MODE, SOUND_DRIVER_CHANNEL, 0);
  ledc_update_duty(SOUND_DRIVER_MODE, SOUND_DRIVER_CHANNEL);
  vTaskDelay(pdMS_TO_TICKS(10));
}

static void sound_task(void *pvParameters) {
  while (1) {
    if (current_track != NULL) {

      for (size_t i = 0; i < current_track->order_len; i++) {
        uint8_t phrase_index = current_track->order[i];
        const Note_t *current_phrase = current_track->phrases[phrase_index];

        int note_idx = 0;
        while (current_phrase[note_idx].duration_ms != DURATION_END) {
          Note_t note = current_phrase[note_idx];

          play_note(note);

          note_idx++;
        }
      }

      // comment to infinte loop
      current_track = NULL;

    } else {
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}