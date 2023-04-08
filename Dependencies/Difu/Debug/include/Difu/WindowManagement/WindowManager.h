#pragma once

#include <raylib.h>
#include <iostream>

namespace WindowManager
{
	bool InitWindow(const std::string& title, int width, int height, bool resizable);
	void RunWindow();
	void CloseWindow();

	Vector2 GetWindowSize();
} // namespace WindowManager
