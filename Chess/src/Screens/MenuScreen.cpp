#include "MenuScreen.h"
#include "../Variables.h"
#include "../Globals.hpp"
#include "../ScreenManagement/ScreenManager.h"
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
    static const int fps[] = {-1, 240, 120, 60, 30};
    static const int fpsSize = 5;

    static Rectangle settingClose;
    static Rectangle settingsFps;
    static Rectangle settingsRenderFPS;

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
	SetTargetFPS(fps[Variables::TargetFPSIndex]);

    startRect = {100, 200, 200, 50};
    loadRect = {100, 270, 200, 50};
    settRect = {100, 340, 200, 50};
    fileDialogState = InitGuiFileDialog(400, 300, (std::string(GetWorkingDirectory()) + "/Assets/Saves").c_str(), false);
    settings = false;

    settingClose = {1, 1, 110, 50};
    settingsFps = {GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f - 25, 170, 50};
    settingsRenderFPS = {GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f + 55, 50, 50};
}

void Unload()
{
}

void Update(float)
{
    if (!fileDialogState.fileDialogActive && !settings)
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, startRect))
            {
                ScreenManager::ChangeScreen(GameScreen::GetScreen());
            }
            else if (CheckCollisionPointRec(mousePos, loadRect))
            {
                fileDialogState.fileDialogActive = true;
            }
            else if (CheckCollisionPointRec(mousePos, settRect))
            {
                settings = true;
            }
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
    {
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePos, settingClose))
            {
                settings = false;
            }

            if (CheckCollisionPointRec(mousePos, settingsFps))
            {
                Variables::TargetFPSIndex += 1;
                if (Variables::TargetFPSIndex >= fpsSize)
                    Variables::TargetFPSIndex = 0;

                SetTargetFPS(fps[Variables::TargetFPSIndex]);
                Logger::Info("Target FPS set to: {}", fps[Variables::TargetFPSIndex]);
            }
            if (CheckCollisionPointRec(mousePos, settingsRenderFPS))
            {
                Variables::RenderFPS = !Variables::RenderFPS;
                Logger::Info("Toggled debug FPS rendering");
            }
        }
    }
}

static void RenderButton(const std::string& text, Rectangle rec)
{
    DrawRectangleLinesEx(rec, 3.0f, Globals::Colors::BUTTON);
    int length = MeasureText(text.c_str(), 32);
    DrawText(text.c_str(), (int)rec.x + ((int)rec.width - length) / 2, (int)rec.y + (int)rec.height / 4, 32, Globals::Colors::BUTTON);
}

static void RenderSettingButton(const std::string& inner, const std::string& outer, Rectangle rec)
{
    int outerLength = MeasureText(outer.c_str(), 32);
    DrawText(outer.c_str(), rec.x - outerLength - 10, rec.y + rec.height / 4 , 32, WHITE);
    DrawRectangleLinesEx(rec, 3.0f, Globals::Colors::SETTINGS_BUTTON);
    int innerLength = MeasureText(inner.c_str(), 32);
    DrawText(inner.c_str(), (int)rec.x + ((int)rec.width - innerLength) / 2, (int)rec.y + (int)rec.height / 4, 32, Globals::Colors::SETTINGS_BUTTON);
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
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Globals::Colors::SETTINGS_BACKGROUND);

        std::string fpsText;
        if (fps[Variables::TargetFPSIndex] <= 0)
            fpsText = "Unlimited";
        else
            fpsText = std::to_string(fps[Variables::TargetFPSIndex]);
        RenderSettingButton(fpsText, "FPS: ", settingsFps);
        if (Variables::RenderFPS)
            RenderSettingButton("x", "Draw FPS: ", settingsRenderFPS);
        else
            RenderSettingButton("", "Draw FPS: ", settingsRenderFPS);
        RenderSettingButton("Close", "", settingClose);
    }

    GuiFileDialog(&fileDialogState);

    if (Variables::RenderFPS)
        DrawFPS(0, 0);
}

void OnResize()
{
	settingsFps = {GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f - 25, 170, 50};
    settingsRenderFPS = {GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f + 55, 50, 50};
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
