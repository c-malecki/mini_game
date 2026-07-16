idf_component_register(SRCS "main.c" "../lib/display.c" "../lib/tracks.c"
                    INCLUDE_DIRS "." "../lib"
                    REQUIRES driver sh1106 esp_driver_ledc esp_adc esp_lcd esp_timer nvs_flash esp_driver_i2c)
