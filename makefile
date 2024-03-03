#بسم الله الرحمن الرحيم

all: dikr settings

dikr:
	gcc src/dikr.c `pkg-config --cflags --libs sdl2 SDL2_ttf` -o bin/dikr
settings:
	gcc src/settings.c `pkg-config --cflags --libs sdl2 SDL2_ttf` -lm -o bin/settings

