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
    static GuiFileDialogState fileDialogState;

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

        return screen;
    }


void Load()
{
    startRect = {100, 200, 200, 50};
    loadRect = {100, 300, 200, 50};
    fileDialogState = InitGuiFileDialog(400, 300, (std::string(GetWorkingDirectory()) + "/Assets/Saves").c_str(), false);
}

void Unload()
{
}

void Update(float dt)
{
    if (!fileDialogState.fileDialogActive)
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
}

void Render()
{
    ClearBackground(Globals::BACKGROUND_COLOR);
    DrawText("Chess", 100, 50, 50, WHITE);
    DrawRectangleLinesEx(startRect, 3.0f, {54, 93, 201, 255});
    DrawText("Start game", (int)startRect.x + 9, (int)startRect.y + 11, 32, {54, 93, 201, 255});
    DrawRectangleLinesEx(loadRect, 3.0f, {54, 93, 201, 255});
    DrawText("Load game", (int)loadRect.x + 9, (int)loadRect.y + 11, 32, {54, 93, 201, 255});

    GuiFileDialog(&fileDialogState);
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
