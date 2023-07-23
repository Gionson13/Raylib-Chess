#include "MenuScreen.h"

#include "Variables.h"
#include "Globals.hpp"
#include "GameScreen.h"
#include "Layers/SettingsLayer.h"

#include <Difu/ScreenManagement/ScreenManager.h>
#include <Difu/WindowManagement/WindowManager.h>

#include <algorithm>
#include <raylib.h>
#include <raymath.h>
#include <cmath>

#ifdef _DEBUG
#include <ctime>
#include <iomanip>
#endif

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#undef RAYGUI_IMPLEMENTATION            // Avoid including raygui implementation again
#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "Utils/GuiFileDialog.h"


namespace MenuScreen
{
	static Rectangle startRect;
	static Rectangle loadRect;
	static Rectangle settRect;
	static Rectangle exitRect;
	static GuiWindowFileDialogState fileDialogState;
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
	fileDialogState = InitGuiWindowFileDialog((std::string(GetWorkingDirectory()) + "/Assets/Saves").c_str());
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
#ifdef _DEBUG
	if (IsKeyPressed(KEY_S))
	{
    	auto time = std::time(nullptr);
    	auto localtime = *std::localtime(&time);

		std::ostringstream oss;
		oss << std::put_time(&localtime, "%d-%m-%Y_%H-%M-%S");

		std::string filename = "Screenshots/" + oss.str() + ".png";
		TakeScreenshot(filename.c_str());
	}
#endif

	if (!fileDialogState.windowActive && !settings)
	{
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			Vector2 mousePos = GetMousePosition();
			if (CheckCollisionPointRec(mousePos, startRect))
				ScreenManager::ChangeScreen(GameScreen::GetScreen());
			else if (CheckCollisionPointRec(mousePos, loadRect))
				fileDialogState.windowActive = true;
			else if (CheckCollisionPointRec(mousePos, settRect))
				settings = true;
			else if (CheckCollisionPointRec(mousePos, exitRect))
				WindowManager::CloseWindow();
		}
		if (IsKeyPressed(KEY_ESCAPE))
			WindowManager::CloseWindow();
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
    int text_width = MeasureText("Chess", 60);
	DrawText("Chess", GetScreenWidth() / 2 - text_width / 2, 50, 60, WHITE);
	RenderButton("Start game", startRect);
	RenderButton("Load game", loadRect);
	RenderButton("Settings", settRect);
	RenderButton("Exit", exitRect);

	if (settings)
	{
		settingsLayer.Render();
	}

	GuiWindowFileDialog(&fileDialogState);

	if (Variables::RenderFPS)
		DrawFPS(0, 0);
}

void OnResize(int width, int height)
{
	settingsLayer.Resize(width, height);

	float rects_x = width / 2.0f - 100.0f;
	const float button_dist = 10.0f;

	startRect = {rects_x, height / 2.0f - 3.0f * button_dist / 2.0f - 100.0f, 200, 50};
	loadRect = {rects_x, height / 2.0f - button_dist / 2.0f - 50.0f, 200, 50};
	settRect = {rects_x, height / 2.0f + button_dist / 2.0f, 200, 50};
	exitRect = {rects_x, height / 2.0f + 3.0f * button_dist / 2.0f + 50.0f, 200, 50};
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

	int stripes_num = 7;
	for (int i = 0; i < stripes_num; i++)
	{
		if (i % 2 == 0)
			DrawRectangle(0, (int)(i * GetScreenHeight() / stripes_num), (int)(x * GetScreenWidth()), (int)(GetScreenHeight() / stripes_num) + 1, BLACK);
		else
			DrawRectangle(GetScreenWidth() - (int)(x * GetScreenWidth()), (int)(i * GetScreenHeight() / stripes_num), (int)(x * GetScreenWidth()), (int)(GetScreenHeight() / stripes_num) + 1, BLACK);
	}
}


bool IsEndTransitionDone(float time)
{
	return time / 2.2f > 1.0f;
}

} // namespace MenuScreen
