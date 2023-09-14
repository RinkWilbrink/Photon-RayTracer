#pragma once
#include "RayTracer/Material/Textures/Noise/PerlinNoise.h"

class NoiseTexture : public Texture
{
private:
	PerlinNoise m_Noise = PerlinNoise();
	float m_Scale = 1.0f;

public:
	NoiseTexture(float _Scale) : m_Scale(_Scale) { }

	virtual Colour Value(float u, float v, const Point3& p) const override
	{
		return Vector::One() * 0.5f * (1 + sin(m_Scale * p.z + 10 * m_Noise.Turbulent(p)));
	}
};