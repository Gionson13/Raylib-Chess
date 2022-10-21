#include "Random.h"

#include <time.h>

Random::Random()
{
	baseSeed = time(NULL);
	seed = baseSeed;
}

Random::Random(uint32_t seed) : baseSeed(seed), seed(seed)
{
}

uint32_t Random::Get()
{
	seed = (seed * 22695477 + 1) % 4294967296;
	return seed;
}

uint32_t Random::GetInt(uint32_t max)
{
	return Get() % max;
}

float Random::GetFloat()
{
	return (float)Get()/(float)(RANDOM_MAX);
}

int Random::GetRange(int min, int max)
{
	return Get() % (max - min + 1) + min;
}

float Random::GetRangef(float min, float max)
{
	return GetFloat() * (max - min) + min;
}

uint32_t Random::GetSeed()
{
	return baseSeed;
}
