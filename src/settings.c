//بسم الله الرحمن الرحيم
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include <SDL2/SDL.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "../submodules/Nuklear/nuklear.h"
#include "../submodules/Nuklear/demo/sdl_renderer/nuklear_sdl_renderer.h"

int main()
{
    //popping_dikr vars
    int popping_dikr_screen_height = 0;
    int popping_dikr_screen_width = 0;
    int popping_dikr_window_width = 400;
    int popping_dikr_window_height = 100;
    int popping_dikr_sleep_minutes = 20; //minutes
    int popping_dikr_display_seconds = 5;
    SDL_Color popping_dikr_window_background_color = {255, 255, 255, 255};
    SDL_Color popping_dikr_dikr_font_color = {.r=0, .g=0, .b=0, .a=255};
    char popping_dikr_dikr_font_path[1024];
    popping_dikr_dikr_font_path[0] = '\0';
    strcat(popping_dikr_dikr_font_path, "/nix/store/7s5v8lcmb38dbsfp6g7nvizdj2p0875v-kacst-2.01/share/fonts/kacst/KacstPoster.ttf");


    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("popping dikr settings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
    if(NULL == window)
    {
        SDL_Log("error SDL_CreateWindow %s", SDL_GetError());
        exit(-1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(NULL == renderer)
    {
        SDL_Log("error SDL_CreateRenderer %s", SDL_GetError());
        exit(-1);
    }


    struct nk_context *ctx = nk_sdl_init(window, renderer);
    {
        struct nk_font_atlas *atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font *font;

        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_default(atlas, 13, &config);

        nk_sdl_font_stash_end();

        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(ctx, &font->handle);
    }

    while(1)
    {
        SDL_Event event;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) goto cleanup;
            nk_sdl_handle_event(&event);
        }
        nk_input_end(ctx);

        int screen_width, screen_height;
        SDL_GetWindowSize(window, &screen_width, &screen_height);
        if(nk_begin(ctx, "bismi_allah", nk_rect(0, 0, screen_width, screen_height), 0/*no flags*/))
        {
            nk_layout_row_static(ctx, 30, 200, 1);
            if (nk_button_label(ctx, "in the name of Allah"))
                fprintf(stdout, "in the name of Allah\n");
            
        }nk_end(ctx);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        nk_sdl_render(NK_ANTI_ALIASING_ON);

        SDL_RenderPresent(renderer);
    }

cleanup:
    nk_sdl_shutdown();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
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
    }else perror("config file error ");
}
*/

