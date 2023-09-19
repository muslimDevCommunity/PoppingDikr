//بسم الله الرحمن الحيم

#include <SDL.h>
#include <SDL_ttf.h>

#include <fstream>
#include <vector>

#ifdef __linux__
  #include <filesystem>
  #include <unistd.h>
#elif _WIN32
  #include <windows.h>
#endif

//
bool running = true;
std::string Settings_path;

// window variables
int cooldown_minutes = 1;
int screen_width = 0, screen_heigth = 0;
int window_width = 250, window_height = 60;
//timer
const uint8_t max_pop_up_time = 15;
long pop_up_time;

SDL_Color BG_color = SDL_Color{255, 255, 255};

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

// Dikr variables
SDL_Texture *Dikr_Texture = nullptr;
SDL_Color Dikr_color = SDL_Color{0, 0, 0};

TTF_Font* Dikr_font = NULL;

int font_size = 60;
short selected_Dikr = 0;
short selected_Font_id = 0;

std::vector<std::string> Dikr_font_vec_ar = {
#ifdef __linux__
  "/usr/share/fonts/truetype/kacst/KacstPoster.ttf",
  "/usr/share/fonts/truetype/kacst/KacstScreen.ttf",
  "/usr/share/fonts/truetype/kacst/KacstQurn.ttf"
#elif _WIN32
  "C:\\Users\\ouham\\AppData\\Local\\Microsoft\\Windows\\Fonts\\KacstPoster.ttf",
  "C:\\Users\\ouham\\AppData\\Local\\Microsoft\\Windows\\Fonts\\KacstQurn.ttf",
  "C:\\Users\\ouham\\AppData\\Local\\Microsoft\\Windows\\Fonts\\KacstTitle.ttf",
  "C:\\Users\\ouham\\AppData\\Local\\Microsoft\\Windows\\Fonts\\KacstScreen.ttf",
  "C:\\Windows\\Fonts\\ARABTYPE.ttf"
#endif
};


/*  word in hex format for Arabic
_Allah \uFEEA\uFEE0\uFEDF\uFE8D
_Ilaha \uFEEA\uFEDFإ
_Mohammed \uFEAA\uFEE4\uFEA4\uFEE3
_Bismi \uFEE2\uFEB4\uFE91
_Sub7an \uFEE6\uFEA4\uFE92\uFEB3
_Akbar \uFEAE\uFE92\uFEDBأ
_Astaghfiro \uFEAE\uFED4\uFED0\uFE98\uFEB3أ
_salla \uFEF0\uFEE0\uFEBB
_ala \uFEF0\uFEE0\uFECB
_la ﻼ
_illa لاإ
*/

std::string Dikr_list_ar[6] = {
  u8"\uFEEA\uFEE0\uFEDF\uFE8D \uFEE2\uFEB4\uFE91", //BismiAllah
  u8"\uFEEA\uFEE0\uFEDF\uFE8D \uFEE6\uFEA4\uFE92\uFEB3", //Sub7ana Allah
  u8"\uFEAA\uFEE4\uFEA4\uFEE3 \uFEF0\uFEE0\uFECB \uFEEA\uFEE0\uFEDF\uFE8D \uFEF0\uFEE0\uFEBB", // salla Allah ala Mohammed
  u8"\uFEEA\uFEE0\uFEDF\uFE8D \uFEAE\uFED4\uFED0\uFE98\uFEB3أ", //astaghfiro Allah
  u8"\uFEEA\uFEE0\uFEDF\uFE8D ﻻإ \uFEEA\uFEDFإ ﻻ", //la ilaha illa Allah
  u8"\uFEAE\uFE92\uFEDBأ \uFEEA\uFEE0\uFEDF\uFE8D" //Allah Akbar
};

void initialize();
void get_settings_path();
void load_settings();
void load_font();
void make_Dikr_texture();
void pop_Dikr();
void clean_up();
void cooldown();

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0){
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }


    if(TTF_Init() == -1)
    {
        printf("Error: %s\n", TTF_GetError());
        return -1;
    }

#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    initialize();
    load_font();

    while(running)
    {
      pop_Dikr();
      clean_up();
      load_settings();
      cooldown();
    }

    SDL_Quit();
}

void initialize(){
    //get screen resolution
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    screen_heigth = DM.h;
    screen_width = DM.w;

    get_settings_path();
    load_settings();
    srand(time(NULL));
}

//Draw the windows (pop-up)
void pop_Dikr(){
  load_settings();
  //time it popped up
  pop_up_time = time(NULL);

    window = SDL_CreateWindow("Dikr", screen_width - window_width, screen_heigth * 3 / 10, window_width, window_height, SDL_WINDOW_POPUP_MENU | SDL_WINDOW_BORDERLESS);
    renderer = SDL_CreateRenderer(window, -1, 0);

    if(renderer == nullptr){
        SDL_Log("Error creating SDL_Renderer!");
    }

    make_Dikr_texture();    
    
    bool w_running = true;
    
    if(NULL == Dikr_Texture || NULL == Dikr_font)
    {
      return;
    }

    SDL_Event w_event;
    while(w_running)
    {
        while(SDL_PollEvent(&w_event))
        {
            if(w_event.type == SDL_QUIT || w_event.type == SDL_MOUSEBUTTONDOWN)
            {
                w_running = false;
            }
        }
        //looking if 15 secs has passed
        if(time(NULL) > (pop_up_time + max_pop_up_time))
        {
          w_running = false;
          break;
        }
        //clear window
        SDL_SetRenderDrawColor(renderer, BG_color.r, BG_color.g, BG_color.b, 255);
        SDL_RenderClear(renderer);
        
        //Draw Dikr
        if(nullptr != Dikr_Texture)
        {
          SDL_RenderCopy(renderer, Dikr_Texture, NULL, NULL);
        }


        SDL_RenderPresent(renderer);
    }
}

void clean_up(){
    SDL_DestroyTexture(Dikr_Texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void load_font()
{
  Dikr_font = TTF_OpenFont(Dikr_font_vec_ar[selected_Font_id].c_str() ,font_size);
  if (NULL != Dikr_font)
  {
    return;
  }

  for (int i = 0; i < Dikr_font_vec_ar.size(); i++) 
  {
    Dikr_font = TTF_OpenFont(Dikr_font_vec_ar[i].c_str() ,font_size);
    if (NULL != Dikr_font)
    {
      return;
    }
  }

}

void cooldown()
{
   SDL_Delay(cooldown_minutes * 60 * 1000);
}

void make_Dikr_texture()
{
  if(NULL == Dikr_font)
  {
    load_font();
  }
  if(NULL == Dikr_font)
  {
    return;
  }


  SDL_Surface * Dikr_Surface = TTF_RenderUTF8_Blended(Dikr_font, Dikr_list_ar[selected_Dikr].data(), Dikr_color);
  Dikr_Texture = SDL_CreateTextureFromSurface(renderer, Dikr_Surface);

  SDL_FreeSurface(Dikr_Surface);
  
  //to make the first dik to be 'BismiAllah' we only randomize after the first time
  selected_Dikr = rand() % 6;
}

void load_settings(){
  std::ifstream SettingsFile;
  SettingsFile.open(Settings_path.c_str(), std::ifstream::in);
  if(SettingsFile.is_open())
  {
    int f_cooldown_time = cooldown_minutes;
    if(SettingsFile >> f_cooldown_time)
    {
      cooldown_minutes = f_cooldown_time;
    }

    int bg_r = BG_color.r, bg_g = BG_color.g, bg_b = BG_color.b;
    if(SettingsFile >> bg_r && SettingsFile >> bg_g && SettingsFile >> bg_b)
    {
      BG_color.r = bg_r;
      BG_color.g = bg_g;
      BG_color.b = bg_b;
    }

    int Dikr_r = Dikr_color.r, Dikr_g = Dikr_color.g, Dikr_b = Dikr_color.b;
    if(SettingsFile >> Dikr_r && SettingsFile >> Dikr_g && SettingsFile >> Dikr_b)
    {
      Dikr_color.r = Dikr_r;
      Dikr_color.g = Dikr_g;
      Dikr_color.b = Dikr_b;
    }

    SettingsFile >> selected_Font_id;

    SettingsFile.close();
  }
  else
  {
    printf("Allah Akbar:Error loading settings\n");
  }
}

void get_settings_path()
{
#ifdef __linux__
  std::string path = "/home/";
  path.append(getlogin());

  
  std::string Settings_dir = path;
  Settings_dir.append("/.PoppingDikr");
  std::filesystem::create_directory(Settings_dir.c_str());


  path.append("/.PoppingDikr/Settings");
  Settings_path = path;
#elif _WIN32
  std::string path = "C:\\Users\\";
  
  char user_name[257];
  DWORD user_name_length = sizeof(user_name);
  GetUserName(user_name, &user_name_length);
  
  path.append(user_name);
  path.append("\\AppData\\Roaming\\popping-dikr");

  CreateDirectory(path.c_str(), NULL);
  path.append("\\settings");
  Settings_path = path;
#endif
}

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  return main();
}
#endif

