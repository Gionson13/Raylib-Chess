#pragma once

#if !defined(RL_VECTOR2_TYPE)
// Vector2 type
typedef struct Vector2
{
    float x;
    float y;
} Vector2;
#define RL_VECTOR2_TYPE
#endif

#if !defined(RL_RECTANGLE_TYPE)
// Rectangle, 4 components
typedef struct Rectangle
{
    float x;      // Rectangle top-left corner position x
    float y;      // Rectangle top-left corner position y
    float width;  // Rectangle width
    float height; // Rectangle height
} Rectangle;
#define RL_RECTANGLE_TYPE
#endif

#if !defined(RL_COLOR_TYPE)
typedef struct Color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;
#define RL_COLOR_TYPE
#endif

#ifndef PI
    #define PI 3.14159265358979323846f
#endif

#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif

#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif


// Vector2 operators ------------------------------------------------------------------------------------------

inline Vector2 operator+(const Vector2& a, const Vector2& b)
{
    return {a.x + b.x, a.y + b.y};
}

inline Vector2 operator+(const Vector2& a, const float value)
{
    return {a.x + value, a.y + value};
}

inline Vector2 operator-(const Vector2& a, const Vector2& b)
{
    return {a.x - b.x, a.y - b.y};
}

inline Vector2 operator-(const Vector2& a, const float value)
{
    return {a.x - value, a.y - value};
}

inline Vector2 operator*(const Vector2& a, const Vector2& b)
{
    return {a.x * b.x, a.y * b.y};
}

inline Vector2 operator*(const Vector2& a, const float value)
{
    return {a.x * value, a.y * value};
}

inline Vector2 operator/(const Vector2& a, const Vector2& b)
{
    return {a.x / b.x, a.y / b.y};
}

inline Vector2 operator/(const Vector2& a, const float value)
{
    return {a.x / value, a.y / value};
}

inline void operator+=(Vector2& a, const Vector2& b)
{
    a = {a.x + b.x, a.y + b.y};
}

inline void operator-=(Vector2& a, const Vector2& b)
{
    a = {a.x - b.x, a.y - b.y};
}

inline void operator*=(Vector2& a, const Vector2& b)
{
    a = {a.x * b.x, a.y * b.y};
}

inline void operator/=(Vector2& a, const Vector2& b)
{
    a = {a.x / b.x, a.y / b.y};
}

inline void operator+=(Vector2& a, const float value)
{
    a = {a.x + value, a.y + value};
}

inline void operator-=(Vector2& a, const float value)
{
    a = {a.x - value, a.y - value};
}

inline void operator*=(Vector2& a, const float value)
{
    a = {a.x * value, a.y * value};
}

inline void operator/=(Vector2& a, const float value)
{
    a = {a.x / value, a.y / value};
}

// -----------------------------------------------------------------------------------------------------------------
// Rectangle Functions ---------------------------------------------------------------------------------------------

inline Vector2 GetRectanglePosition(const Rectangle& rectangle)
{
    return { rectangle.x, rectangle.y };
}

inline Vector2 GetRectangleSize(const Rectangle& rectangle)
{
    return { rectangle.width, rectangle.height };
}

// -----------------------------------------------------------------------------------------------------------------
// Color Functions -------------------------------------------------------------------------------------------------

inline Color LerpColor(Color start, Color end, float progress)
{
    int r = start.r + progress * (end.r - start.r);
    int g = start.g + progress * (end.g - start.g);
    int b = start.b + progress * (end.b - start.b);
    int a = start.a + progress * (end.a - start.a);
    return {(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a};
}
