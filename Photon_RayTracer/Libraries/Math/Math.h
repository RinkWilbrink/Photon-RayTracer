#pragma once

#include <cmath>
#include <cstdlib>

// Constants
const float infinity = std::numeric_limits<float>::infinity();
const float  pi = 3.1415927; // 3.1415926535897932385

const float Epsilon = 0.001f;

//utility functions

inline float degrees_to_radians(float degrees)
{
	return degrees * pi / 180.0f;
}

inline float random_float()
{
	// Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0f);
}

inline float random_float(float min, float max)
{
	// Returns a random real in [min,max).
	return min + (max - min) * random_float();
}

inline int RandomInt(int min, int max)
{
	return static_cast<int>(random_float(static_cast<float>(min), static_cast<float>(max + 1.0f)));
}

inline float clamp(float x, float min, float max)
{
	if (x < min)
	{
		return min;
	}
	if (x > max)
	{
		return max;
	}
	return x;
}