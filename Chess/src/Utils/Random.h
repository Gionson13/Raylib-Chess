#pragma once

#include <stdint.h>

#define RANDOM_MAX 4294967295

class Random
{
public:
    Random();
    Random(uint32_t seed);

    // Returns a random uint32_t
    uint32_t Get();
    // Returns a random integer between 0 and max (max not included)
    uint32_t GetInt(uint32_t max);
    // Returns a random float between 0.0 and 1.0
    float GetFloat();
    // Returns a random integer between min and max
    int GetRange(int min, int max);
    // Returns a random float between min and max
    float GetRangef(float min, float max);
    // Returns the seed used when the instance was first created
    uint32_t GetSeed();

private:
    uint32_t baseSeed;
    uint32_t seed;
};
