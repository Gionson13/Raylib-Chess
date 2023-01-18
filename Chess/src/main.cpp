#include "ScreenManagement/ScreenManager.h"
#include "Screens/MenuScreen.h"
#include "WindowManagement/WindowManager.h"
#include <raylib.h>

int main()
{
	if (WindowManager::InitWindow("Chess", 800, 480, true))
	{
		SetExitKey(0);
		ScreenManager::ChangeScreen(MenuScreen::GetScreen());
		WindowManager::RunWindow();
	}
}
