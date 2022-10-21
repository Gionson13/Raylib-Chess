#pragma once

#include "../Utils/Logger.h"
#include "Screen.h"

#include <raylib.h>

namespace ScreenManager
{
	// extern Screen* currentScreen;
	//
	// extern Screen* nextScreen; // TODO: find a way to make these private
	// 
	// extern bool IsInTransition;
	// extern float TransitionBeginTime;
	// namespace
	// {
	//     bool IsInTransition = false;
	//     float transitionBeginTime = 0.0f;
	// }

	void UpdateScreen();
	void RenderScreen();

	void UpdateScreenManager();
	void UpdateAndRenderTransitions();

	void OnResize();

	// template<typename T>
	// void ChangeScreen()
	// {
	//     if (IsInTransition)
	//         return;
	//
	//     nextScreen = new T();
	//     IsInTransition = true;
	//     TransitionBeginTime = GetTime();
	// }

	void ChangeScreen(const Screen screen);
} // namespace ScreenManager
