#include "WindowManager.h"

#include "../ScreenManagement/ScreenManager.h"

bool WindowManager::InitWindow(const std::string& title, int width, int height)
{
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    ::InitWindow(width, height, title.c_str());
    // MaximizeWindow();
    gameRunning = true;

    return true;
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
}

void WindowManager::CloseWindow()
{
    gameRunning = false;
}


Vector2 WindowManager::GetWindowSize()
{
    return {(float)GetScreenWidth(), (float)GetScreenHeight()};
}
