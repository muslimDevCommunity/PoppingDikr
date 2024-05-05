//بسم الله الرحمن الرحيم

#ifdef __unix__
    #include <unistd.h>
#endif

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
#include <SDL2/SDL_ttf.h>

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

#ifdef __linux__
#define SUPPORTED_PLATFORM_TO_SCAN_SYSTEM_FONTS
#endif

int main()
{
    //popping_dikr vars
    int popping_dikr_window_width = 400;
    int popping_dikr_window_height = 100;
    int popping_dikr_sleep_minutes = 20; //minutes
    int popping_dikr_display_seconds = 5;
    SDL_Color popping_dikr_window_background_color = {255, 255, 255, 255};
    SDL_Color popping_dikr_dikr_font_color = {.r=0, .g=0, .b=0, .a=255};
    char popping_dikr_dikr_font_path[1024];
    popping_dikr_dikr_font_path[0] = '\0';
    strcat(popping_dikr_dikr_font_path, "res/KacstPoster.ttf");

    int popping_dikr_dikr_font_path_length;

    char popping_dikr_conf_path[1024];
    popping_dikr_conf_path[0] = '\0';
    strcat(popping_dikr_conf_path, ".popping-dikr");

    struct nk_colorf background_color;
    struct nk_colorf font_color;

    #ifdef __linux__
    {
        //getting the config path
        char path[256] = "/home/";
        strcat(path, getlogin());
        strcat(path, "/.popping-dikr");
        popping_dikr_conf_path[0] = '\0';
        strcat(popping_dikr_conf_path, path);
    }
    #endif

    //load the settings
    {
        FILE* file = fopen(popping_dikr_conf_path, "rb");
        if(file)
        {
            int tmp = 0; //to remove errors
            tmp = fread(&popping_dikr_window_width, sizeof(popping_dikr_window_width), 1, file);
            tmp = fread(&popping_dikr_window_height, sizeof(popping_dikr_window_height), 1, file);
            tmp = fread(&popping_dikr_sleep_minutes, sizeof(popping_dikr_sleep_minutes), 1, file);
            tmp = fread(&popping_dikr_display_seconds, sizeof(popping_dikr_display_seconds), 1, file);
            tmp = fread(&popping_dikr_window_background_color, sizeof(popping_dikr_window_background_color), 1, file);
            tmp = fread(&popping_dikr_dikr_font_color, sizeof(popping_dikr_dikr_font_color), 1, file);
            char *str = fgets(popping_dikr_dikr_font_path, sizeof(popping_dikr_dikr_font_path), file);
            fclose(file);
        }
        else perror("config file ");
    }
    popping_dikr_dikr_font_path_length = strlen(popping_dikr_dikr_font_path);

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
    if(TTF_Init() == -1)
    {
        printf("Error: %s\n", TTF_GetError());
        return -1;
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

        //does not need to be inside this block
        //but it's here for clean experiance
        font_color.r = (float)popping_dikr_dikr_font_color.r / 255.0f;
        font_color.g = (float)popping_dikr_dikr_font_color.g / 255.0f;
        font_color.b = (float)popping_dikr_dikr_font_color.b / 255.0f;
        font_color.a = 1.0f;
        background_color.r = (float)popping_dikr_window_background_color.r / 255.0f;
        background_color.g = (float)popping_dikr_window_background_color.g / 255.0f;
        background_color.b = (float)popping_dikr_window_background_color.b / 255.0f;
        background_color.a = 1.0f;
    }
    
    static char *buffer = NULL;                                 //buffer to hold the files read
    static char **font_path_array = NULL;                       //array of null terminated paths
    static int font_count = 1;                                  //number of fonts

    #ifdef __linux__
    //TODO make this depend on platform
    system("fc-list > /tmp/fc-list_scan");
    int file_size;
    FILE *fp = fopen("/tmp/fc-list_scan", "r");
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    buffer = (char*)malloc(file_size);
    rewind(fp);
    font_path_array = malloc(sizeof(char**));
    font_path_array[0] = buffer;
    for(int i = 0; i < file_size; i++)
    {
        char c;
        if(EOF == (c = fgetc(fp))){break;}
            buffer[i] = c;
        if('\n' == c)
        {
            font_count += 1;
            font_path_array = realloc(font_path_array, font_count * sizeof(char**));
            font_path_array[font_count - 1] = (char*)(buffer + i + 1);
        }
        else if(':' == c)
        {
            buffer[i] = '\0';
        }
    }
    fclose(fp);
    SDL_Texture *font_image_array[font_count];
    {
        SDL_Color dikr_font_color = {.r=255, .g=255, .b=255, .a=255};
        for (int i =0; i<font_count; i++)
        {
            TTF_Font *dikr_font = TTF_OpenFont(font_path_array[i], 64);
            if(NULL == dikr_font)
            {
                printf("[SDL_TTF] font is NULL %s\n", TTF_GetError());
            }
            SDL_Surface* dikr_surface = TTF_RenderUTF8_Blended(dikr_font, "\uFEEA\uFEE0\uFEDF\uFE8D ﻻإ \uFEEA\uFEDFإ ﻻ", dikr_font_color);
            font_image_array[i] = SDL_CreateTextureFromSurface(renderer, dikr_surface);

            TTF_CloseFont(dikr_font);
            SDL_FreeSurface(dikr_surface);
        }
    }
    #endif


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

        int window_width, window_height;
        SDL_GetWindowSize(window, &window_width, &window_height);
        if(nk_begin(ctx, "bismi_allah", nk_rect(0, 0, window_width, window_height), 0/*no flags*/))
        {
            //window dimensions
            nk_layout_row_dynamic(ctx, 30, 2);
            nk_property_int(ctx, "window width", 0, &popping_dikr_window_width, 1000, 1, 1);
            nk_property_int(ctx, "window height", 0, &popping_dikr_window_height, 1000, 1, 1);

            //diplay seconds
            //sleep minutes
            nk_layout_row_dynamic(ctx, 30, 2);
            nk_property_int(ctx, "display seconds", 0, &popping_dikr_display_seconds, 1000, 1, 1);
            nk_property_int(ctx, "sleep minutes", 0, &popping_dikr_sleep_minutes, 1000, 1, 1);

            //font colors
            nk_layout_row_static(ctx, 25, 200, 2);
            nk_label(ctx, "font color", NK_TEXT_LEFT);
            if(nk_combo_begin_color(ctx, nk_rgb_cf(font_color), nk_vec2(200, 200)))
            {
                nk_layout_row_dynamic(ctx, 120, 1);
                font_color = nk_color_picker(ctx, font_color, NK_RGBA);
                nk_combo_end(ctx);
            }
            //font path
            //TODO change the font path selector into something like a shocase of all fonts 'a rendered texture with that font'
            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, "font path", NK_TEXT_LEFT);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, popping_dikr_dikr_font_path, &popping_dikr_dikr_font_path_length, sizeof(popping_dikr_dikr_font_path), nk_filter_default);

            #ifdef SUPPORTED_PLATFORM_TO_SCAN_SYSTEM_FONTS
            static int show_font = 0;
            nk_layout_row_dynamic(ctx, 0, 1);
            nk_checkbox_label(ctx, "show fonts list", &show_font);

            if(show_font)
            {
                nk_layout_row_dynamic(ctx, 300, 1);
                if(nk_group_begin(ctx, "group fonts", 0))
                {
                    //nk_layout_row_dynamic(ctx, 25, 1);
                    nk_layout_row_static(ctx, popping_dikr_window_height, popping_dikr_window_width, 3);
                    for(int i = 0; i < font_count; i++)
                    {
                        //TODO only show this button if it is a ttf
                        if(NULL == strstr(font_path_array[i], ".ttf")) continue;
                        //if(nk_button_label(ctx, font_path_array[i]))
                        if(NULL == font_image_array[i]) continue;
                        if(nk_button_image(ctx, nk_image_ptr(font_image_array[i])))
                        {
                            popping_dikr_dikr_font_path[0] = '\0';
                            strcat(popping_dikr_dikr_font_path, font_path_array[i]);
                            popping_dikr_dikr_font_path_length = strlen(popping_dikr_dikr_font_path);
                        }
                    }
                    nk_group_end(ctx);
                }
            }
            #endif

            //background color
            nk_layout_row_static(ctx, 25, 200, 2);
            nk_label(ctx, "background color", NK_TEXT_LEFT);
            if(nk_combo_begin_color(ctx, nk_rgb_cf(background_color), nk_vec2(200, 200)))
            {
                nk_layout_row_dynamic(ctx, 120, 1);
                background_color = nk_color_picker(ctx, background_color, NK_RGBA);
                nk_combo_end(ctx);
            }

            //save
            nk_layout_row_static(ctx, 30, 200, 1);
            if(nk_button_label(ctx, "save"))
            {
                popping_dikr_dikr_font_color.r = font_color.r * 255;
                popping_dikr_dikr_font_color.g = font_color.g * 255;
                popping_dikr_dikr_font_color.b = font_color.b * 255;
                popping_dikr_dikr_font_color.a = font_color.a * 255;

                popping_dikr_window_background_color.r = background_color.r * 255;
                popping_dikr_window_background_color.g = background_color.g * 255;
                popping_dikr_window_background_color.b = background_color.b * 255;
                popping_dikr_window_background_color.a = background_color.a * 255;

                FILE* file = fopen(popping_dikr_conf_path, "wb");
                if(file)
                {
                fwrite(&popping_dikr_window_width, sizeof(popping_dikr_window_width), 1, file);
                fwrite(&popping_dikr_window_height, sizeof(popping_dikr_window_height), 1, file);
                fwrite(&popping_dikr_sleep_minutes, sizeof(popping_dikr_sleep_minutes), 1, file);
                fwrite(&popping_dikr_display_seconds, sizeof(popping_dikr_display_seconds), 1, file);
                fwrite(&popping_dikr_window_background_color, sizeof(popping_dikr_window_background_color), 1, file);
                fwrite(&popping_dikr_dikr_font_color, sizeof(popping_dikr_dikr_font_color), 1, file);
                char c = popping_dikr_dikr_font_path[popping_dikr_dikr_font_path_length + 1];
                popping_dikr_dikr_font_path[popping_dikr_dikr_font_path_length] = '\0';
                fputs(popping_dikr_dikr_font_path, file);
                popping_dikr_dikr_font_path[popping_dikr_dikr_font_path_length] = c;
                fclose(file);
                }else perror("config file error ");
            }
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

