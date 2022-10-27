#include "MenuScreen.h"
#include "../Variables.h"
#include "../Globals.hpp"
#include "../ScreenManagement/ScreenManager.h"
#include "../Layers/SettingsLayer.h"
#include "GameScreen.h"

#include <algorithm>
#include <raylib.h>
#include <raymath.h>
#include <cmath>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#undef RAYGUI_IMPLEMENTATION            // Avoid including raygui implementation again
#define GUI_FILE_DIALOG_IMPLEMENTATION
#include "../Utils/GuiFileDialog.h"

namespace MenuScreen
{
	static Rectangle startRect;
	static Rectangle loadRect;
	static Rectangle settRect;
	static GuiFileDialogState fileDialogState;
	static bool settings;

	static Layer settingsLayer;

	Screen GetScreen()
	{
		Screen screen;
		screen.LoadFunction = &Load;
		screen.UnloadFunction = &Unload;
		screen.UpdateFunction = &Update;
		screen.RenderFunction = &Render;
		screen.IsEndTransitionDoneFunction = &IsEndTransitionDone;
		screen.IsStartTransitionDoneFunction = &IsStartTransitionDone;
		screen.RenderStartTransitionFunction = &RenderStartTransition;
		screen.RenderEndTransitionFunction = &RenderEndTransition;
		screen.OnResize = &OnResize;

		return screen;
	}


void Load()
{
	startRect = {100, 200, 200, 50};
	loadRect = {100, 270, 200, 50};
	settRect = {100, 340, 200, 50};
	fileDialogState = InitGuiFileDialog(400, 300, (std::string(GetWorkingDirectory()) + "/Assets/Saves").c_str(), false);
	settings = false;

	settingsLayer = SettingsLayer::GetLayer();
	settingsLayer.x = 0;
	settingsLayer.y = 0;
	settingsLayer.Load();
}

void Unload()
{
	settingsLayer.Unload();
}

void Update(float dt)
{
	if (!fileDialogState.fileDialogActive && !settings)
	{
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			Vector2 mousePos = GetMousePosition();
			if (CheckCollisionPointRec(mousePos, startRect))
				ScreenManager::ChangeScreen(GameScreen::GetScreen());
			else if (CheckCollisionPointRec(mousePos, loadRect))
				fileDialogState.fileDialogActive = true;
			else if (CheckCollisionPointRec(mousePos, settRect))
				settings = true;
		}
	}

	if (fileDialogState.SelectFilePressed)
	{
		// Load image file (if supported extension)
		if (IsFileExtension(fileDialogState.fileNameText, ".csv"))
		{
			std::string fileName = TextFormat("%s/%s", fileDialogState.dirPathText, fileDialogState.fileNameText);
			Variables::BoardFilePath = fileName;
			ScreenManager::ChangeScreen(GameScreen::GetScreen());
		}

		fileDialogState.SelectFilePressed = false;
	}

	if (settings)
		settings = !settingsLayer.Update(dt);
}

static void RenderButton(const std::string& text, Rectangle rec)
{
	DrawRectangleLinesEx(rec, 3.0f, Globals::Colors::BUTTON);
	int length = MeasureText(text.c_str(), 32);
	DrawText(text.c_str(), (int)rec.x + ((int)rec.width - length) / 2, (int)rec.y + (int)rec.height / 4, 32, Globals::Colors::BUTTON);
}

void Render()
{
	ClearBackground(Globals::Colors::BACKGROUND);
	DrawText("Chess", 100, 50, 50, WHITE);
	RenderButton("Start game", startRect);
	RenderButton("Load game", loadRect);
	RenderButton("Settings", settRect);

	if (settings)
	{
		settingsLayer.Render();
	}

	GuiFileDialog(&fileDialogState);

	if (Variables::RenderFPS)
		DrawFPS(0, 0);
}

void OnResize(int width, int height)
{
	settingsLayer.Resize(width, height);
}

void RenderStartTransition(float time)
{
	float progress = time / 1.3f;
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, (unsigned char)(int)Lerp(255, 0, progress)});
}

bool IsStartTransitionDone(float time)
{
	return time / 1.3f > 1.0f;
}


void RenderEndTransition(float time)
{
	float progress = time / 2.0f;

	float x = std::min(std::pow(progress, 3) * 4 + std::pow(progress, 2), 8.0f * std::pow(progress - 0.77f,2) + 0.65f);
    
	for (int i = 0; i < 6; i++)
	{
		if (i % 2 == 0)
			DrawRectangle(0, (int)(i * GetScreenHeight() / 6), (int)(x * GetScreenWidth()), (int)(GetScreenHeight() / 6) + 1, BLACK);
		else
			DrawRectangle(GetScreenWidth() - (int)(x * GetScreenWidth()), (int)(i * GetScreenHeight() / 6), (int)(x * GetScreenWidth()), (int)(GetScreenHeight() / 6) + 1, BLACK);
	}
}


bool IsEndTransitionDone(float time)
{
	return time / 2.2f > 1.0f;
}

} // namespace MenuScreen
