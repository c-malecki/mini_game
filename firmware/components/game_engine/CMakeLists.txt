idf_component_register(
    SRCS "game_engine.c" "src/controls.c"
    INCLUDE_DIRS "include"
    REQUIRES driver esp_driver_ledc esp_timer
    PRIV_REQUIRES device_registry
)