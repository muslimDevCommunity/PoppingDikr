//بسم الله الرحمن الرحيم

#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
    SDL_Color window_background_color = {255, 255, 255, 255};
    SDL_Color dikr_font_color = {.r=0, .g=0, .b=0, .a=255};
    int display_seconds = 5;
    char conf_path[1024];
    conf_path[0] = '\0';
    strcat(conf_path, ".popping-dikr");

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    if(TTF_Init() == -1)
    {
        printf("Error: %s\n", TTF_GetError());
        return -1;
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

    
    bismi_allah:;
    //config file
    //1. ~/.popping-dikr
    //const char* dikr_font_path = ;
    char dikr_font_path[1024];
    dikr_font_path[0] = '\0';
    strcat(dikr_font_path, "res/KacstPoster.ttf");

/*
 * file structure by the will of Allah
 * window_width - window_height - sleep_minutes - display_seconds - bg_color - dikr_color - font_path
*/
    //load settings
    #ifdef __linux__
    {
        char path[256] = "/home/";
        strcat(path, getlogin());
        strcat(path, "/.popping-dikr");
        conf_path[0] = '\0';
        strcat(conf_path, path);
    }
    #endif

    FILE* file = fopen(conf_path, "rb");
    if(file)
    {
        int tmp = 0;
        tmp = fread(&window_width, sizeof(window_width), 1, file);
        tmp = fread(&window_height, sizeof(window_height), 1, file);
        tmp = fread(&sleep_minutes, sizeof(sleep_minutes), 1, file);
        tmp = fread(&display_seconds, sizeof(display_seconds), 1, file);
        tmp = fread(&window_background_color, sizeof(window_background_color), 1, file);
        tmp = fread(&dikr_font_color, sizeof(dikr_font_color), 1, file);
        char *str = fgets(dikr_font_path, sizeof(dikr_font_path), file);
        fclose(file);
    }
    else perror("config file ");
    //get screen size
    {
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        screen_height = dm.h;
        screen_width = dm.w;
    }

    //make window and renderer
    SDL_Window* window = SDL_CreateWindow("dikr", screen_width - window_width, (int)screen_height*3/10, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_POPUP_MENU | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, window_background_color.r, window_background_color.g, window_background_color.b, window_background_color.a);
    SDL_RenderClear(renderer);

    //load fonts and create dikr
    printf("dikr_font_path: %s\n", dikr_font_path);
    TTF_Font* dikr_font = TTF_OpenFont(dikr_font_path, 100);
    if(NULL == dikr_font)
    {
        printf("[SDL_TTF] font is NULL %s\n", TTF_GetError());
    }
    SDL_Surface* dikr_surface = TTF_RenderUTF8_Blended(dikr_font, dikr_list[dikr_list_selected_index], dikr_font_color);
    SDL_Texture* dikr_texture = SDL_CreateTextureFromSurface(renderer, dikr_surface);

    SDL_RenderCopy(renderer, dikr_texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    //pop the dikr and hide when it is pressed
    SDL_Delay(display_seconds * 1000);

    //cleen up
    SDL_FreeSurface(dikr_surface);
    SDL_DestroyTexture(dikr_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    dikr_list_selected_index = rand() % (sizeof(dikr_list) / sizeof(dikr_list[0]));

    SDL_Delay(sleep_minutes * 60 * 1000);
    goto bismi_allah;

    SDL_Quit();

}

