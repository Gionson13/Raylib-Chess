# include "SettingsLayer.h"

#include "../Variables.h"
#include "../Globals.hpp"
#include "../Utils/Logger.h"

namespace SettingsLayer
{
	static const int fps[] = {-1, 240, 120, 60, 30};
	static const int fpsSize = 5;

	static int w, h;

	static Rectangle settingClose;
	static Rectangle settingsFps;
	static Rectangle settingsRenderFPS;
	static Rectangle settingsVsync;

	static void Load()
	{
		SetTargetFPS(fps[Variables::TargetFPSIndex]);
		
		settingClose = {1, 1, 110, 50};
		settingsFps = {GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f - 95, 170, 50};
		settingsRenderFPS = {GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f - 25, 50, 50};
		settingsVsync = {GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f + 45, 50, 50};
	}

	static void Unload()
	{
	}

	static bool Update(float dt)
	{
		(void)dt;
		bool result = false;
		Vector2 mousePos = GetMousePosition();
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			if (CheckCollisionPointRec(mousePos, settingClose))
			{
				result = true;
			}

			if (CheckCollisionPointRec(mousePos, settingsFps))
			{
				Variables::TargetFPSIndex += 1;
				if (Variables::TargetFPSIndex >= fpsSize)
					Variables::TargetFPSIndex = 0;

				SetTargetFPS(fps[Variables::TargetFPSIndex]);
				LOG_INFO("Target FPS set to: {}", fps[Variables::TargetFPSIndex]);
			}
			if (CheckCollisionPointRec(mousePos, settingsRenderFPS))
			{
				Variables::RenderFPS = !Variables::RenderFPS;
				LOG_INFO("Toggled debug FPS rendering");
			}
			if (CheckCollisionPointRec(mousePos, settingsVsync))
			{
				if (IsWindowState(FLAG_VSYNC_HINT))
					ClearWindowState(FLAG_VSYNC_HINT);
				else	
					SetWindowState(FLAG_VSYNC_HINT);
				
				LOG_INFO("Toggled VSYNC");
			}
		}

		return result;
	}
	static void RenderSettingButton(const std::string& inner, const std::string& outer, Rectangle rec)
	{
		int outerLength = MeasureText(outer.c_str(), 32);
		DrawText(outer.c_str(), rec.x - outerLength - 10, rec.y + rec.height / 4 , 32, WHITE);
		DrawRectangleLinesEx(rec, 3.0f, Globals::Colors::SETTINGS_BUTTON);
		int innerLength = MeasureText(inner.c_str(), 32);
		DrawText(inner.c_str(), (int)rec.x + ((int)rec.width - innerLength) / 2, (int)rec.y + (int)rec.height / 4, 32, Globals::Colors::SETTINGS_BUTTON);
	}
	
	static void Render()
	{
		DrawRectangle(0, 0, w, h, Globals::Colors::SETTINGS_BACKGROUND);

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
		if (IsWindowState(FLAG_VSYNC_HINT))
			RenderSettingButton("x", "Vsync: ", settingsVsync);
		else
			RenderSettingButton("", "Vsync: ", settingsVsync);
		RenderSettingButton("Close", "", settingClose);
	}

	static void OnResize(int width, int height)
	{
		w = width;
		h = height;
		settingsFps = {width / 2.0f - 50, height / 2.0f - 95, 170, 50};
		settingsRenderFPS = {width / 2.0f - 50, height / 2.0f - 25, 50, 50};
		settingsVsync = {width / 2.0f - 50, height / 2.0f + 45, 50, 50};
	}


	Layer GetLayer()
	{
		Layer result;
		
		result.OnLoad = &Load;
		result.OnUnload = &Unload;
		result.OnUpdate = &Update;
		result.OnRender = &Render;
		result.OnResize = &OnResize;

		return result;
	}
}
