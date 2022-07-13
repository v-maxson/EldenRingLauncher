#include "gui.h"
#include "gui_style.h"

#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>


constexpr const char* WINDOW_TITLE = "Elden Ring Launcher";
constexpr unsigned int WINDOW_WIDTH = 300;
constexpr unsigned int WINDOW_HEIGHT = 150;
constexpr SDL_WindowFlags WINDOW_FLAGS = (SDL_WindowFlags)(
    SDL_WINDOW_ALLOW_HIGHDPI 
    | SDL_WINDOW_BORDERLESS
    );

constexpr SDL_RendererFlags RENDERER_FLAGS = (SDL_RendererFlags)(
    SDL_RENDERER_ACCELERATED
    );

static bool g_sdlInitialized = false;
static bool g_imguiInitialized = false;

static SDL_Window* g_window = nullptr;
static SDL_Renderer* g_renderer = nullptr;

bool Gui::InitializeSDL() noexcept {
    // Initialize SDL Window
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return false;

    g_window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        WINDOW_FLAGS);

    if (!g_window)
        return false;

    g_renderer = SDL_CreateRenderer(g_window, -1, RENDERER_FLAGS);

    if (!g_renderer)
        return false;

    SDL_SetWindowHitTest(
        g_window, 
        [](SDL_Window* win, const SDL_Point* area, void* data) -> SDL_HitTestResult {
            // Allow window to be dragged from top bar.
            if (area->x >= 0
                && area->x <= WINDOW_WIDTH - 20
                && area->y >= 0
                && area->y <= 19)
                return SDL_HITTEST_DRAGGABLE;
            

            return SDL_HITTEST_NORMAL;
        }, 
        nullptr);
     
    g_sdlInitialized = true;
    return true;
}

bool Gui::InitializeImGui() noexcept {
    // SDL must be initialized.
    if (!g_sdlInitialized)
        return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = nullptr;

    Gui::StyleColors();
    Gui::StyleFonts();

    if (!ImGui_ImplSDL2_InitForSDLRenderer(g_window, g_renderer))
        return false;

    if (!ImGui_ImplSDLRenderer_Init(g_renderer))
        return false;
}

void Gui::DestroySDL() noexcept {
    if (!g_sdlInitialized)
        return;

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}

void Gui::DestroyImGui() noexcept {
    if (!g_imguiInitialized)
        return;

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Gui::BeginRender() noexcept {
    ImGui_ImplSDL2_NewFrame();
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui::NewFrame();
}

void Gui::EndRender() noexcept {
    ImGui::Render();
    SDL_RenderClear(g_renderer);
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(g_renderer);
}

void Gui::Render() noexcept {
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ WINDOW_WIDTH, WINDOW_HEIGHT });

    ImGui::Begin(
        WINDOW_TITLE,
        &Gui::g_shouldLoop,
        ImGuiWindowFlags_NoSavedSettings 
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse
    );

    if (ImGui::Button("Start Online", ImVec2(WINDOW_WIDTH - 27, (WINDOW_HEIGHT / 2) - 35))) {
        // Launch the original start_protected_game.exe
    }

    ImGui::Separator();

    if (ImGui::Button("Start Offline", ImVec2(WINDOW_WIDTH - 27, (WINDOW_HEIGHT / 2) - 35))) {
        // Launch eldenring.exe directly.
    }

    ImGui::End();
}

void Gui::Run() {
    Gui::InitializeSDL();
    Gui::InitializeImGui();

    while (Gui::g_shouldLoop) {
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            ImGui_ImplSDL2_ProcessEvent(&evt);

            switch (evt.type) {
                // Handle quiting.
                // Note: This only applies to bordered windows.
                case SDL_QUIT: {
                    Gui::g_shouldLoop = false;
                } break;
            }
        }

        Gui::BeginRender();
        Gui::Render();
        Gui::EndRender();
    }

    Gui::DestroyImGui();
    Gui::DestroySDL();
}
