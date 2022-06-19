#pragma once

#include <raylib.h>

// TODO: Add support for textures
// TODO: Add support for multiple shapes

struct Particle
{
    float life = 0.0f;

    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float rotation;
    float rotationVelocity;
    float rotationAcceleration;

    Color startColor;
    Color endColor;
    Vector2 size;
};
