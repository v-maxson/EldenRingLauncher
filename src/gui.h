#pragma once

namespace Gui {
	static bool g_shouldLoop = true;

	// Initialization
	bool InitializeSDL() noexcept;
	bool InitializeImGui() noexcept;

	// Cleanup
	void DestroySDL() noexcept;
	void DestroyImGui() noexcept;

	// Rendering
	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;

	void Run();
}

