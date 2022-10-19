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
	float lifeDuration;
	float lifetime = 0.0f;

	std::string text;

	Vector2 startPosition;
	Vector2 endPosition;

	int textSize;
	Color textColor;
	Color bannerColor;

	bool started = false;
};
