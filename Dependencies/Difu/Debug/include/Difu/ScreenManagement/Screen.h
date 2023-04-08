#pragma once

struct Screen
{
	// Mandatory
	void(*LoadFunction)() = nullptr;
	void(*UnloadFunction)() = nullptr;
	void(*UpdateFunction)(float) = nullptr;
	void(*RenderFunction)() = nullptr;

	// Optional
	bool(*IsStartTransitionDoneFunction)(float) = nullptr;
	bool(*IsEndTransitionDoneFunction)(float) = nullptr;
	void(*RenderStartTransitionFunction)(float) = nullptr;
	void(*RenderEndTransitionFunction)(float) = nullptr;

	// Callbacks (reccomended)
	void(*OnResize)(int, int) = nullptr;
};
