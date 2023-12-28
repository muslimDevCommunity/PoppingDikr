//بسم الله الرحمن الرحيم

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//no functions by the will od Allah

int main()
{
    printf("بسم الله الرحمن الرحيم\n");

    int screen_height = 0;
    int screen_width = 0;
    int window_width = 400;
    int window_height = 100;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    if(TTF_Init() == -1)
    {
        printf("Error: %s\n", TTF_GetError());
    }

    //bismi_allah:
    //load settings

    //pop the dikr app
    SDL_Window* window = SDL_CreateWindow("dikr", 0, 0, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_POPUP_MENU | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    TTF_Font* dikr_font = TTF_OpenFont("/usr/share/fonts/truetype/kacst/KacstPoster.ttf", 100);
    SDL_Color dikr_font_color = {.r=0, .g=0, .b=0};
    SDL_Surface* dikr_surface = TTF_RenderUTF8_Blended(dikr_font, "\uFEEA\uFEE0\uFEDF\uFE8D ﻻإ \uFEEA\uFEDFإ ﻻ", dikr_font_color);
    SDL_Texture* dikr_texture = SDL_CreateTextureFromSurface(renderer, dikr_surface);

    SDL_RenderCopy(renderer, dikr_texture, NULL, NULL);

    SDL_RenderPresent(renderer);
    SDL_Delay(2 * 1000);
    SDL_Quit();

}

