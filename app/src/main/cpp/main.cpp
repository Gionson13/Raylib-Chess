#include "Difu/ScreenManagement/ScreenManager.h"
#include "Chess/Screens/MenuScreen.h"
#include "Difu/WindowManagement/WindowManager.h"
#include <raylib.h>

int main(int argc, char* argv[])
{
	if (WindowManager::InitWindow("Chess", 0, 0, true))
	{
//		SetExitKey(0);
		ScreenManager::ChangeScreen(MenuScreen::GetScreen());
		WindowManager::RunWindow();
	}
}
