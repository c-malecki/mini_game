idf_component_register(
    SRCS "game_engine.c" "src/controls.c" "src/display.c" "src/sound.c" "src/tracks.c"
    INCLUDE_DIRS "include"
    REQUIRES driver esp_lcd esp_driver_ledc esp_driver_i2c esp_timer sh1106
)