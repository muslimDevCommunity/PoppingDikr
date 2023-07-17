//بسم الله الرحمن الرحيم

#include <SDL_ttf.h>
#include "imgui_SDL_includes.h"
#include <fstream>

#include <iostream>
#include <vector>

#include <filesystem>

//#ifdef __linux__
#include <unistd.h>
//#endif

int cooldown_minutes = 0;

std::string Settings_path;

int w_width = 480, w_height = 400;
float BG_color[3] = {0,0,0};
float Dikr_color[3] = {0,0,0};
const char* Dikr_Preview = u8"\uFEEA\uFEE0\uFEDF\uFE8D ﻻإ \uFEEA\uFEDFإ ﻻ"; //La ilaha Illa Allah

TTF_Font* Dikr_font = nullptr;
SDL_Texture* Preview_Texture = nullptr;


bool error_loading_settings = false;

std::vector<std::string> Dikr_font_vec = {
  "/usr/share/fonts/truetype/kacst/KacstPoster.ttf",
  "/usr/share/fonts/truetype/kacst/KacstScreen.ttf",
  "/usr/share/fonts/truetype/kacst/KacstQurn.ttf"
};


void init();
void set_theme();
void get_settings_path();
void load_font();
void read_settings();
void write_settings();
void make_app_run_on_boot();
void show_settings();
void RenderPreview(SDL_Renderer* renderer);
void frame_cleenup();

// Main code
int main()
{

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    if(TTF_Init() == -1)
    {
        printf("Error: %s\n", TTF_GetError());
        return -1;
    }
    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("BismiAllah PoppingDikr Settings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_width, w_height, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    init();

    // Main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        show_settings();

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        RenderPreview(renderer);
        
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
        frame_cleenup();
    }

    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void show_settings()
{
  ImGui::SetNextWindowPos(ImVec2{0, 0});
  ImGui::SetNextWindowSize(ImVec2{(float)(w_width), (float)(w_height)});

  ImGui::Begin("BismiAllah", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
  if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
  {
    if(ImGui::BeginTabItem("general"))
    {
      ImGui::SliderInt("cooldown minutes", &cooldown_minutes, 1 ,120);

      ImGui::ColorEdit3("Background Color", BG_color);
      ImGui::ColorEdit3("Dikr Color", Dikr_color);

      if(error_loading_settings)
      {
        ImGui::TextColored(ImVec4(1, 1, 0, 1) ,"error reading files!\ntry save and then reload settings :)\nmay Allah bless you");
      }

      if(ImGui::Button("reload settings"))
      {
        read_settings();
      }

      if(ImGui::Button("save"))
      {
        write_settings();
      }

      if(ImGui::Button("Make it run on boot"))
      {
        make_app_run_on_boot();
      }
      ImGui::EndTabItem();
    }
    
    if(ImGui::BeginTabItem("fonts"))
    {
      for(int i = 0; i < Dikr_font_vec.size(); i++)
      {
        if(ImGui::Button(Dikr_font_vec[i].data()))
        {
          std::cout << "Incha2Allah will select ffont: " << Dikr_font_vec[0] << '\n';
        }
      }

      ImGui::EndTabItem();
    }

    if(ImGui::BeginTabItem("Window"))
    {
      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }  

  ImGui::End();
}

void read_settings(){
  std::ifstream SettingsFile;
  SettingsFile.open(Settings_path.c_str(), std::ifstream::in);
  if(SettingsFile.is_open())
  {
    int f_cooldown_time = cooldown_minutes;
    if(SettingsFile >> f_cooldown_time)
    {
      cooldown_minutes = f_cooldown_time;
    }

    float bg_r = 0, bg_g = 0, bg_b = 0;
    if(SettingsFile >> bg_r && SettingsFile >> bg_g && SettingsFile >> bg_b)
    {
      BG_color[0] = bg_r / 255;
      BG_color[1] = bg_g / 255;
      BG_color[2] = bg_b / 255;
    }

    float Dikr_r = 0, Dikr_g = 0, Dikr_b = 0;
    if(SettingsFile >> Dikr_r && SettingsFile >> Dikr_g && SettingsFile >> Dikr_b)
    {
      Dikr_color[0] = Dikr_r / 255;
      Dikr_color[1] = Dikr_g / 255;
      Dikr_color[2] = Dikr_b / 255;
    }
    SettingsFile.close();
    error_loading_settings = false;
  }
  else
  {
    error_loading_settings = true;
  }
}

void write_settings()
{
  std::ofstream SettingsFile;
  SettingsFile.open(Settings_path.c_str());

  SettingsFile << cooldown_minutes << '\n';
  SettingsFile << int(BG_color[0] * 255) << ' ' << int(BG_color[1] * 255) << ' ' << int(BG_color[2] * 255) << '\n';
  SettingsFile << int(Dikr_color[0] * 255) << ' ' << int(Dikr_color[1] * 255) << ' ' << int(Dikr_color[2] * 255) << '\n';
  SettingsFile.close();

}

void init()
{
  get_settings_path();
  read_settings();
  load_font();
  set_theme();
}

void set_theme()
{
  ImGuiStyle& style = ImGui::GetStyle();
  style.FrameRounding = 9.0f;

  style.FramePadding = ImVec2(5, 5);
  style.FrameRounding = 2.0f;

  style.ItemSpacing = ImVec2(04, 06);
  style.ItemInnerSpacing = ImVec2(12, 6);

  style.GrabMinSize = 9.0f;
  style.GrabRounding = 2.0f;
}

void get_settings_path()
{
  std::string path = "/home/";
  path.append(getlogin());

  
  std::string Settings_dir = path;
  Settings_dir.append("/.PoppingDikr");
  std::filesystem::create_directory(Settings_dir.c_str());


  path.append("/.PoppingDikr/Settings");
  Settings_path = path;
}

void RenderPreview(SDL_Renderer* renderer)
{
  if (NULL == Dikr_font)
  {
    printf("Allah Akbar: Dikr Font in NULL\n");
    load_font();
  }

  SDL_Rect preview_rect = SDL_Rect{w_width - 250, w_height - 60, 250, 60};
  SDL_Color Preview_Dikr_Color = SDL_Color{(Uint8)(Dikr_color[0] * 255.0f), (Uint8)(Dikr_color[1] * 255.0f), (Uint8)(Dikr_color[2] * 255.0f)};
  
  //Make Preview Texture
  SDL_Surface * Dikr_Surface = TTF_RenderUTF8_Solid(Dikr_font, Dikr_Preview, Preview_Dikr_Color);
  Preview_Texture = SDL_CreateTextureFromSurface(renderer, Dikr_Surface);

  if (nullptr == Dikr_Surface)
  {
    printf("Allah Akbar: Dikr_Surface is nullptr\n");
  }
  if (nullptr == Preview_Texture)
  {
    printf("Allah Akbar: Preview_Texture is NULL\n");
  }

  SDL_FreeSurface(Dikr_Surface);

  //Render Dikr Preview
  SDL_SetRenderDrawColor(renderer, (Uint8)(BG_color[0] * 255.0f), (Uint8)(BG_color[1] * 255.0f), (Uint8)(BG_color[2] * 255.0f), 255);
  SDL_RenderFillRect(renderer, &preview_rect);
  SDL_RenderCopy(renderer, Preview_Texture, NULL, &preview_rect);
}

void load_font()
{
  for (int i = 0; i < 3; i++) 
  {
    Dikr_font = TTF_OpenFont(Dikr_font_vec[i].data() ,60);
    if (NULL != Dikr_font)
    {
      return;
    }
    std::cout << "Allah Akbar: Error loading font: " << Dikr_font_vec[i] << '\n';
  }
}

void frame_cleenup()
{
  SDL_DestroyTexture(Preview_Texture);
}

void make_app_run_on_boot()
{
  const char* dot_desktop_content = "[Desktop Entry]\n"
  "Version=1.0\n"
  "Type=Application\n"
  "Terminal=false\n"
  "Exec=/usr/bin/popping-dikr\n"
  "Name=Popping Dikr\n";
  
  std::string path = "/home/";
  path.append(getlogin());

  path.append("/.config/autostart/PoppingDikr.desktop");

  std::ofstream dot_desktop_file;
  dot_desktop_file.open(path.c_str());
  dot_desktop_file << dot_desktop_content;

}

