#بسم الله الرحمن الرحيم

all: dikr

dikr:
	gcc src/dikr.c -l SDL2 -l SDL2_ttf -o bin/dikr

