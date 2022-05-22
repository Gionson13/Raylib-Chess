#pragma once

#include <iostream>
#include <raylib.h>

class BannerAnimation
{
public:
    BannerAnimation();
    BannerAnimation(float duration, std::string text, Vector2 start, Vector2 end, int fontSize, Color textColor, Color bannerColor);

    void Update(float dt);
    void Render();

    bool IsDone();

    void Start();

private:
   Vector2 startPosition;
   Vector2 endPosition;

   Color bannerColor;
   Color textColor;
   std::string text;
   int textSize;

   float lifeDuration;
   float lifetime = 0.0f;

   bool started = false;
};
