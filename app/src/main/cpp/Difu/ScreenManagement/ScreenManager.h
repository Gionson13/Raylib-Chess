#pragma once

#include "../Utils/Logger.h"
#include "../ScreenManagement/Screen.h"

#include <raylib.h>

namespace ScreenManager
{
	void UpdateScreen();
	void RenderScreen();

	void UpdateScreenManager();
	void UpdateAndRenderTransitions();

	void OnResize();

	void ChangeScreen(const Screen screen);
} // namespace ScreenManager
