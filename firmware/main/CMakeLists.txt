idf_component_register(SRCS "main.c" "../lib/galaga/galaga.c" "../lib/galaga/sprites.c"
                    INCLUDE_DIRS "." "../lib/galaga"
                    REQUIRES game_engine)
