#pragma once

#include <raylib.h>

// TODO: Add support for textures
// TODO: Add support for multiple shapes

struct Particle
{
    float lifetime;

    Vector2 position;
    Vector2 velocity;
    float rotation;
    float rotationVelocity;

    Color startColor;
    Color endColor;
    Vector2 size;
};
