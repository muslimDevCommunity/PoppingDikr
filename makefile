#بسم الله الرحمن الرحيم

all: dikr

dikr:
	gcc src/dikr.c `pkg-config --cflags --libs sdl2 SDL2_ttf` -o bin/dikr

