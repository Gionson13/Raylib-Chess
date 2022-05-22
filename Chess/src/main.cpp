#include "ScreenManagement/ScreenManager.h"
#include "Screens/MenuScreen.h"
#include "WindowManagement/WindowManager.h"

int main()
{
    if (WindowManager::InitWindow("Chess", 800, 480))
    {
        ScreenManager::ChangeScreen<MenuScreen>();
        WindowManager::RunWindow();
    }
}
