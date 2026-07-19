idf_component_register(
    SRCS "device_system.c" "src/battery.c"
    INCLUDE_DIRS "include"
    REQUIRES driver esp_adc esp_timer
)