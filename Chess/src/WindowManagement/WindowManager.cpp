#include "WindowManager.h"

#include "../ScreenManagement/ScreenManager.h"
#include "../Utils/Logger.h"


bool WindowManager::InitWindow(const std::string& title, int width, int height)
{
    SetTraceLogLevel(LOG_WARNING);

    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    ::InitWindow(width, height, title.c_str());
    // MaximizeWindow();
    gameRunning = true;

    if (IsWindowReady())
    {
        LOG_INFO("Window successfully created:\n\tTitle:\t{}\n\tSize: \t{}x{}", title.c_str(), width, height);
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
    while (gameRunning)
    {
        ScreenManager::UpdateScreenManager(); // Change Screen if necessary

        gameRunning = !WindowShouldClose();
        ScreenManager::UpdateScreen(); 

        BeginDrawing();
        ScreenManager::RenderScreen();
        ScreenManager::UpdateAndRenderTransitions();
        EndDrawing();
    }

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
