#include "BannerAnimation.h"
#include "raylib.h"

#include <raymath.h>

static float smootherstep(float progress)
{
    return Clamp(6 * powf(progress, 5.0f) - 15 * powf(progress, 4.0f) + 10 * powf(progress, 3.0f), 0, 1);
}


BannerAnimation::BannerAnimation()
{
    lifeDuration = 0.0f;
    text = "";
    startPosition = {0.0f, 0.0f};
    endPosition = {0.0f, 0.0f};
    textSize = 0;
    textColor = WHITE;
    bannerColor = WHITE;
}

BannerAnimation::BannerAnimation(float duration, std::string text, Vector2 start, Vector2 end, int fontSize, Color textColor, Color bannerColor) : lifeDuration(duration), text(text), startPosition(start), endPosition(end), textSize(fontSize), textColor(textColor), bannerColor(bannerColor)
{
}

void BannerAnimation::Update(float dt)
{
    if (started)
        lifetime += dt;
}

void BannerAnimation::Render()
{
    if (started)
    {
        float progress = lifetime / lifeDuration;
        
        Vector2 position;

        float x1 = smootherstep(2 * progress) / 2;
        float x2 = smootherstep(2 * progress - 1) / 2;
        float x = x1 + x2;

        int textLength = MeasureText(text.c_str(), textSize);
        position.x = startPosition.x + (endPosition.x - startPosition.x) * x - textLength / 2.0f - 10.0f;
        position.y = startPosition.y + (endPosition.y - startPosition.y) * x - 8.0f;
    
        DrawRectangleV(position, {textLength + 20.0f, textSize + 16.0f}, bannerColor);
        DrawText(text.c_str(), (int)position.x + 10, (int)position.y + 8, textSize, textColor);
    }
}

bool BannerAnimation::IsDone()
{
    return lifetime > lifeDuration;
}

void BannerAnimation::Start()
{
    started = true;
}
