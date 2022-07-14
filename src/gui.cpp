#include "gui.h"
#include "gui_style.h"

#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <Windows.h>
#include <thread>
using namespace std::literals::chrono_literals;

constexpr const char* WINDOW_TITLE = "Elden Ring Launcher";
constexpr unsigned int WINDOW_WIDTH = 300;
constexpr unsigned int WINDOW_HEIGHT = 150;
constexpr SDL_WindowFlags WINDOW_FLAGS = (SDL_WindowFlags)(
    SDL_WINDOW_ALLOW_HIGHDPI 
    | SDL_WINDOW_BORDERLESS
    );

constexpr const char* ER_LAUNCH_OPTIONS = "-eac-nop-loaded";

constexpr SDL_RendererFlags RENDERER_FLAGS = (SDL_RendererFlags)(
    SDL_RENDERER_ACCELERATED
    );

static bool g_sdlInitialized = false;
static bool g_imguiInitialized = false;
static bool g_shouldLoop = true;

static SDL_Window* g_window = nullptr;
static SDL_Renderer* g_renderer = nullptr;

bool InitializeSDL() noexcept {
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

bool InitializeImGui() noexcept {
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

void DestroySDL() noexcept {
    if (!g_sdlInitialized)
        return;

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}

void DestroyImGui() noexcept {
    if (!g_imguiInitialized)
        return;

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void BeginRender() noexcept {
    ImGui_ImplSDL2_NewFrame();
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui::NewFrame();
}

void EndRender() noexcept {
    ImGui::Render();
    SDL_RenderClear(g_renderer);
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(g_renderer);
}

void Render() noexcept {
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ WINDOW_WIDTH, WINDOW_HEIGHT });

    ImGui::Begin(
        WINDOW_TITLE,
        &g_shouldLoop,
        ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse
    );

    if (ImGui::Button("Start Online", ImVec2(WINDOW_WIDTH - 27, (WINDOW_HEIGHT / 2) - 35))) {
        // Launch the original start_protected_game.exe
        // It should now be called start_protected_game_original.exe
        STARTUPINFOW sInfo{};
        sInfo.cb = sizeof(sInfo);
        ZeroMemory(&sInfo, sizeof(sInfo));
        PROCESS_INFORMATION pInfo{};
        ZeroMemory(&pInfo, sizeof(pInfo));
        if (!CreateProcessW(
            L"start_protected_game_original.exe",
            nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr,
            &sInfo,
            &pInfo
        )) {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Elden Ring Launcher Error",
                "Could not launch start_protected_game_original.exe",
                g_window
            );
        }

        // Not interested in hProcess or hThread currently.
        if (pInfo.hProcess) CloseHandle(pInfo.hProcess);
        if (pInfo.hThread) CloseHandle(pInfo.hThread);
        
        // Close
        g_shouldLoop = false;
    }

    ImGui::Separator();

    if (ImGui::Button("Start Offline", ImVec2(WINDOW_WIDTH - 27, (WINDOW_HEIGHT / 2) - 35))) {
        // Launch eldenring.exe directly.
        STARTUPINFOW sInfo{};
        sInfo.cb = sizeof(sInfo);
        ZeroMemory(&sInfo, sizeof(sInfo));
        PROCESS_INFORMATION pInfo{};
        ZeroMemory(&pInfo, sizeof(pInfo));
        if (!CreateProcessW(
            L"eldenring.exe",
            (wchar_t*)L"-eac-nop-loaded",
            nullptr, nullptr, FALSE, 0, nullptr, nullptr,
            &sInfo,
            &pInfo
        )) {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Elden Ring Launcher Error",
                "Could not launch eldenring.exe",
                g_window
            );
        }

        // Not interested in hProcess or hThread currently.
        if (pInfo.hProcess) CloseHandle(pInfo.hProcess);
        if (pInfo.hThread) CloseHandle(pInfo.hThread);

        // Close
        g_shouldLoop = false;
    }

    ImGui::End();
}

void Gui::Run() {
    InitializeSDL();
    InitializeImGui();

    while (g_shouldLoop) {
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            ImGui_ImplSDL2_ProcessEvent(&evt);

            switch (evt.type) {
                // Handle quiting.
                // Note: This only applies to bordered windows.
                case SDL_QUIT: {
                    g_shouldLoop = false;
                } break;
            }
        }

        BeginRender();
        Render();
        EndRender();
        
        std::this_thread::sleep_for(50ms);
    }

    DestroyImGui();
    DestroySDL();
}
