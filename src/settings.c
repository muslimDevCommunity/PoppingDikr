//بسم الله الرحمن الرحيم

#include <SDL2/SDL.h>
#include "submodules/Nuklear/nuklear.h"
#include "submodules/Nuklear/demo/sdl_renderer/nuklear_sdl_renderer.h"

int main()
{
}


/*
//in the name of Allah

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int main()
{
    int screen_height = 0;
    int screen_width = 0;
    int window_width = 400;
    int window_height = 100;
    int sleep_minutes = 20; //minutes
    int display_seconds = 5;
    SDL_Color window_background_color = {255, 255, 255, 255};
    SDL_Color dikr_font_color = {.r=0, .g=0, .b=0, .a=255};
    char dikr_font_path[1024];
    dikr_font_path[0] = '\0';
    //strcat(dikr_font_path, "/nix/store/7s5v8lcmb38dbsfp6g7nvizdj2p0875v-kacst-2.01/share/fonts/kacst/KacstDigital.ttf");
    strcat(dikr_font_path, "/nix/store/7s5v8lcmb38dbsfp6g7nvizdj2p0875v-kacst-2.01/share/fonts/kacst/KacstPoster.ttf");

    char conf_path[256] = "/home/";
    strcat(conf_path, getlogin());
    strcat(conf_path, "/.popping-dikr");
    printf("the conf path is : %s\n", conf_path);
    
    FILE* file = fopen(conf_path, "wb");
    if(file)
    {
        fwrite(&window_width, sizeof(window_width), 1, file);
        fwrite(&window_height, sizeof(window_height), 1, file);
        fwrite(&sleep_minutes, sizeof(sleep_minutes), 1, file);
        fwrite(&display_seconds, sizeof(display_seconds), 1, file);
        fwrite(&window_background_color, sizeof(window_background_color), 1, file);
        fwrite(&dikr_font_color, sizeof(dikr_font_color), 1, file);
        fputs(dikr_font_path, file);
        fclose(file);
    }else perror("config file ");
}
*/

