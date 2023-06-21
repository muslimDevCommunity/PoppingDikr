//بسم الله الرحمن الرحيم

#include "imgui_SDL_includes.h"
#include <fstream>

#include <iostream>
#include <filesystem>

int cooldown_minutes = 0;

float BG_color[3] = {0,0,0};
float Dikr_color[3] = {0,0,0};

bool error_loading_settings = false;

void read_settings();
void write_settings();
void show_settings();

// Main code
int main()
{

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("BismiAllah PoppingDikr Settings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
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

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    read_settings();

    // Main loop
    bool done = false;
    while (!done){
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
        
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
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
  ImGui::Begin("BismiAllah", NULL, ImGuiWindowFlags_NoTitleBar);
  

  ImGui::SliderInt("cooldown minutes", &cooldown_minutes, 1 ,60);

  ImGui::ColorEdit3("Background Color", BG_color);
  ImGui::ColorEdit3("Dikr Color", Dikr_color);

  if(error_loading_settings)
  {
    ImGui::TextColored(ImVec4(1, 1, 0, 1) ,"error reading files!");
  }

  if(ImGui::Button("reload settings"))
  {
    read_settings();
  }

  if(ImGui::Button("save"))
  {
    write_settings();
  }

  ImGui::End();
}

void read_settings(){
  std::ifstream SettingsFile;
  SettingsFile.open("files/Settings", std::ifstream::in);
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
  SettingsFile.open("files/Settings");

  SettingsFile << cooldown_minutes << '\n';
  SettingsFile << int(BG_color[0] * 255) << ' ' << int(BG_color[1] * 255) << ' ' << int(BG_color[2] * 255) << '\n';
  SettingsFile << int(Dikr_color[0] * 255) << ' ' << int(Dikr_color[1] * 255) << ' ' << int(Dikr_color[2] * 255) << '\n';
  SettingsFile.close();

}

