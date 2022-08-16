#include "gui.h"
#include "gui_style.h"
#include "app_version.h"

#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <process.hpp>
#include <cstdint>
#include <thread>
#include <format>

constexpr uint32_t SDL_INIT_FLAGS = (
	SDL_INIT_VIDEO
	);

static bool g_isOpen = true;

static SDL_Window* g_window = nullptr;
constexpr SDL_WindowFlags WINDOW_FLAGS = (SDL_WindowFlags)(
	SDL_WINDOW_ALLOW_HIGHDPI
	| SDL_WINDOW_BORDERLESS
	);
constexpr const char* WINDOW_TITLE = "EldenRingLauncher";
constexpr uint32_t WINDOW_WIDTH = 300;
constexpr uint32_t WINDOW_HEIGHT = 150;

static SDL_Renderer* g_renderer = nullptr;
constexpr SDL_RendererFlags RENDERER_FLAGS = (SDL_RendererFlags)(
	SDL_RENDERER_ACCELERATED
	| SDL_RENDERER_PRESENTVSYNC
	);

/// @brief Attempts to initialize required SDL2 components.
///	@return A bool indicating whether or not initialization succeeded.
bool InitializeSDL() noexcept {
	// Initialize required SDL subsystems.
	if (SDL_Init(SDL_INIT_FLAGS) != 0)
		return false;

	// Initialize window.
	g_window = SDL_CreateWindow(
		WINDOW_TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_FLAGS
	);

	if (!g_window)
		return false;

	// Initialize renderer.
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

	return true;
}

/// @brief Attempts to initialize ImGui.
/// @return A bool indicating whether or not initialization succeeded.
bool InitializeImGui() noexcept {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;

	Gui::StyleCustom();

	// This is currently causing a segmentation fault when the program exits. 
	// Gui::StyleFonts();

	if (!ImGui_ImplSDL2_InitForSDLRenderer(g_window, g_renderer))
		return false;

	if (!ImGui_ImplSDLRenderer_Init(g_renderer))
		return false;

	return true;
}

/// @brief SDL Cleanup.
void DestroySDL() noexcept {
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	SDL_Quit();
}

/// @brief ImGui Cleanup.
void DestroyImGui() noexcept {
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void BeginRender() noexcept {
	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui::NewFrame();
}

void Render() noexcept {
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WINDOW_WIDTH, WINDOW_HEIGHT });

	ImGui::Begin(
		std::format("{} v{}", WINDOW_TITLE, VERSION_STRING).c_str(),
		&g_isOpen,
		ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoCollapse
	);

	if (ImGui::Button("Start Online", ImVec2(WINDOW_WIDTH - 27, (WINDOW_HEIGHT / 2) - 35))) {
		// Start the original start_protected_game.exe file.
		auto process = TinyProcessLib::Process("", "start_protected_game.exe");
	}

	ImGui::Separator();

	if (ImGui::Button("Start Offline", ImVec2(WINDOW_WIDTH - 27, (WINDOW_HEIGHT / 2) - 35))) {
		// Start eldenring.exe
		auto process = TinyProcessLib::Process("-eac-nop-loaded", "eldenring.exe");
	}

	ImGui::End();
}

void EndRender() noexcept {
	ImGui::Render();
	SDL_RenderClear(g_renderer);
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(g_renderer);
}

void Gui::Run() {
	if (!InitializeSDL() || !InitializeImGui())
		return;

	while (g_isOpen) {
		SDL_Event event;
		while (SDL_PollEvent(&event))
			ImGui_ImplSDL2_ProcessEvent(&event);

		BeginRender();
		Render();
		EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	DestroyImGui();
	DestroySDL();
}
