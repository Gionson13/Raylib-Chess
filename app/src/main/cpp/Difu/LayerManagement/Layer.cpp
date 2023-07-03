#include "Layer.h"

#include "../Utils/Logger.h"

#include <rlgl.h>

static void DoNothing() {}
static void DoNothing_intint(int, int) {}
static bool ReturnFalse_float(float) { return false; }


void Layer::Load()
{
	if (!OnLoad)
	{
		OnLoad = &DoNothing;
		LOG_WARN("Layer's OnLoad has not been defined");
	}
	if (!OnUnload)
	{
		OnUnload = &DoNothing;
		LOG_WARN("Layer's OnUnload has not been defined");
	}
	if (!OnRender)
	{
		OnRender = &DoNothing;
		LOG_WARN("Layer's OnRender has not been defined");
	}
	if (!OnUpdate)
	{
		OnUpdate = &ReturnFalse_float;
		LOG_WARN("Layer's OnUpdate has not been defined");
	}
	if (!OnResize)
	{
		OnResize = &DoNothing_intint;
		LOG_WARN("Layer's OnResize has not been defined");
	}
	OnLoad();
	OnResize(width, height);
}

void Layer::Unload()
{
	OnUnload();
}


void Layer::Resize(int _width, int _height)
{
	width = _width;
	height = _height;

	OnResize(width, height);
}

bool Layer::Update(float dt)
{
	SetMouseOffset(-x, -y);
	bool result = OnUpdate(dt);

	// TODO: Set to previous offset
	SetMouseOffset(0, 0);
	return result;
}

void Layer::Render()
{
	rlPushMatrix();
	rlTranslatef(x, y, 0);
	OnRender();
	rlPopMatrix();
}
