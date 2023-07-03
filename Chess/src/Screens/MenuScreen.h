#pragma once

#include <Difu/ScreenManagement/Screen.h>
#include <raylib.h>

namespace MenuScreen
{
	Screen GetScreen();

	void Load();
	void Unload();

	void Update(float dt);
	void Render();

	void OnResize(int width, int height);

	void RenderStartTransition(float time);
	bool IsStartTransitionDone(float time);

	void RenderEndTransition(float time);
	bool IsEndTransitionDone(float time);
} // namespace MenuScreen
