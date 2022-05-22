#pragma once

#include <raylib.h>
#include <iostream>

namespace WindowManager
{
    bool InitWindow(const std::string& title, int width, int height);
    void RunWindow();
    void CloseWindow();

    Vector2 GetWindowSize();

    namespace
    {
        bool gameRunning;
    } // namespace
} // namespace WindowManager
