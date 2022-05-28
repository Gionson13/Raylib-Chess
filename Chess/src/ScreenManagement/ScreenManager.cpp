#include "ScreenManager.h"
#include "raylib.h"

namespace ScreenManager
{
    Screen* currentScreen = nullptr;
    Screen* nextScreen = nullptr;
    
    bool IsInTransition = false;
    float TransitionBeginTime = 0.0f;

    void UpdateScreen()
    {
        if (currentScreen)
            currentScreen->Update(GetFrameTime());
    }

    void RenderScreen()
    {
        if (currentScreen)
            currentScreen->Render();
    }

    void UpdateScreenManager()
    {
        if (IsInTransition)
            return;

        if (nextScreen)
        {
            if (currentScreen)
            {
                currentScreen->Unload();
                delete currentScreen;
            }

            currentScreen = nextScreen;
            nextScreen = nullptr;

            currentScreen->Load();
            LOG_INFO("Screen successfully loaded");

            IsInTransition = true;
            TransitionBeginTime = GetTime();
        }
    }

    void UpdateAndRenderTransitions()
    {
        if (!IsInTransition)
            return;

        if (nextScreen)
        {
            if (currentScreen)
            {
                float time = (float)GetTime() - TransitionBeginTime;
                if (!currentScreen->IsEndTransitionDone(time)) 
                    currentScreen->RenderEndTransition(time);
                else
                {
                    currentScreen->RenderEndTransition(time - GetFrameTime());
                    IsInTransition = false;
                }
            }
            else
                IsInTransition = false;
        }
        else
        {
            if (currentScreen)
            {
                float time = (float)GetTime() - TransitionBeginTime;
                if (!currentScreen->IsStartTransitionDone(time))
                    currentScreen->RenderStartTransition(time);
                else
                {
                    currentScreen->RenderStartTransition(time - GetFrameTime());
                    IsInTransition = false;
                }
            }
            else 
            {
                IsInTransition = false;
            }
        }
    }
} // namespace ScreenManager
