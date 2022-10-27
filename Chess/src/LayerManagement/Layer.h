#pragma once

#include <raylib.h>

struct Layer
{
	int x, y, width, height;
	
	void Load();
	void Unload();
	void Resize(int width, int height);
	void Render();
	// Returns whether the layer should end
	[[nodiscard("If the value is true, the layer should close")]] bool Update(float dt);

	void(*OnLoad)() = nullptr;
	void(*OnUnload)() = nullptr;
	bool(*OnUpdate)(float) = nullptr;
	void(*OnRender)() = nullptr;

	void(*OnResize)(int, int) = nullptr;
};
