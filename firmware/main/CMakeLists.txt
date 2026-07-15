idf_component_register(SRCS "mini_game.c" "../lib/display.c""../lib/tracks.c"
                    INCLUDE_DIRS "." "../lib"
                    REQUIRES esp_driver_ledc esp_adc esp_timer nvs_flash esp_driver_i2c)
