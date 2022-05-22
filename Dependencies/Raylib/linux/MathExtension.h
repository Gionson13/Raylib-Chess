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

Vector2 GetRectanglePosition(const Rectangle& rectangle)
{
    return { rectangle.x, rectangle.y };
}

Vector2 GetRectangleSize(const Rectangle& rectangle)
{
    return { rectangle.width, rectangle.height };
}