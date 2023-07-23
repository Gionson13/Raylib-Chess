#include "WindowManager.h"

#include "Difu/ScreenManagement/ScreenManager.h"
#include "Difu/Utils/Logger.h"

#ifdef PLATFORM_WEB
    #include <emscripten/emscripten.h>
#endif

static bool gameRunning;

static void UpdateRenderFrame()
{
	ScreenManager::UpdateScreenManager(); // Change Screen if necessary

	gameRunning = !WindowShouldClose();
	if (IsWindowResized())
		ScreenManager::OnResize();
	ScreenManager::UpdateScreen();

	BeginDrawing();
	ScreenManager::RenderScreen();
	ScreenManager::UpdateAndRenderTransitions();
	EndDrawing();
}

bool WindowManager::InitWindow(const std::string& title, int width, int height, bool resizable)
{
	SetTraceLogLevel(LOG_WARNING);

	if (resizable)
		SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	::InitWindow(width, height, title.c_str());
	// MaximizeWindow();
	gameRunning = true;

	if (IsWindowReady())
	{
		LOG_INFO("Window successfully created:\n\tTitle:\t    {}\n\tSize:\t    {}x{}\n\tResizable:  {}", title, width, height, resizable);
		return true;
	}
	else
	{
		LOG_ERROR("Failed to create window");
		return false;
	}
}

void WindowManager::RunWindow()
{
#ifdef PLATFORM_WEB
	emscripten_set_main_loop(UpdateRenderFrame, 60, 1);
#else
	while (gameRunning)
	{
		UpdateRenderFrame();
	}
#endif

	::CloseWindow();
	LOG_INFO("Window closed successfully");
}

void WindowManager::CloseWindow()
{
	gameRunning = false;
}


Vector2 WindowManager::GetWindowSize()
{
    return {(float)GetScreenWidth(), (float)GetScreenHeight()};
}
