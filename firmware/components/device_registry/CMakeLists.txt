idf_component_register(
    SRCS "device_registry.c" "src/battery.c" "src/display.c" "src/sound.c"
    INCLUDE_DIRS "include"
    REQUIRES driver esp_lcd esp_adc esp_driver_ledc esp_driver_i2c esp_timer sh1106-esp-idf
)