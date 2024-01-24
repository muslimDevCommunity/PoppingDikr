//بسم الله الرحمن الرحيم

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//no functions by the will of Allah

int main()
{
    printf("بسم الله الرحمن الرحيم\n");

    int screen_height = 0;
    int screen_width = 0;
    int window_width = 400;
    int window_height = 100;
    int sleep_minutes = 20; //minutes

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    if(TTF_Init() == -1)
    {
        printf("Error: %s\n", TTF_GetError());
    }

    //they are in unicode form because sdl2_ttf did not format them properly
    const char* dikr_list[] =
    {
        u8"\uFEEA\uFEE0\uFEDF\uFE8D \uFEE2\uFEB4\uFE91", //BismiAllah
        u8"\uFEEA\uFEE0\uFEDF\uFE8D \uFEE6\uFEA4\uFE92\uFEB3", //Sub7ana Allah
        u8"\uFEAA\uFEE4\uFEA4\uFEE3 \uFEF0\uFEE0\uFECB \uFEEA\uFEE0\uFEDF\uFE8D \uFEF0\uFEE0\uFEBB", // salla Allah ala Mohammed
        u8"\uFEEA\uFEE0\uFEDF\uFE8D \uFEAE\uFED4\uFED0\uFE98\uFEB3أ", //astaghfiro Allah
        u8"\uFEEA\uFEE0\uFEDF\uFE8D ﻻإ \uFEEA\uFEDFإ ﻻ", //la ilaha illa Allah
        u8"\uFEAE\uFE92\uFEDBأ \uFEEA\uFEE0\uFEDF\uFE8D" //Allah Akbar
    };

    int dikr_list_selected_index = 0;

    //for the font path: by the will of Allah it is gonna be read from the config file
    const char* dikr_font_path = "/nix/store/7s5v8lcmb38dbsfp6g7nvizdj2p0875v-kacst-2.01/share/fonts/kacst/KacstPoster.ttf";

    bismi_allah:
    //load settings

    //get screen size
    {
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        screen_height = dm.h;
        screen_width = dm.w;
    }

    //pop the dikr app
    SDL_Window* window = SDL_CreateWindow("dikr", screen_width - window_width, (int)screen_height*3/10, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_POPUP_MENU | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    TTF_Font* dikr_font = TTF_OpenFont(dikr_font_path, 100);
    if(NULL == dikr_font)
    {
        printf("[SDL_TTF] font is NULL %s\n", TTF_GetError());
    }
    SDL_Color dikr_font_color = {.r=0, .g=0, .b=0, .a=255};
    SDL_Surface* dikr_surface = TTF_RenderUTF8_Blended(dikr_font, dikr_list[dikr_list_selected_index], dikr_font_color);
    SDL_Texture* dikr_texture = SDL_CreateTextureFromSurface(renderer, dikr_surface);

    SDL_RenderCopy(renderer, dikr_texture, NULL, NULL);

    SDL_RenderPresent(renderer);
    SDL_Delay(2 * 1000);

    //cleen up
    SDL_DestroyTexture(dikr_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    dikr_list_selected_index = rand() % (sizeof(dikr_list) / sizeof(dikr_list[0]));

    SDL_Delay(sleep_minutes * 60 * 1000);
    //SDL_R
    goto bismi_allah;

    SDL_Quit();

}

