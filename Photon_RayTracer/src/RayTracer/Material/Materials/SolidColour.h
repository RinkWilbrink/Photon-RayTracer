#pragma once
#include "RayTracer/Material/Texture.h"

class SolidColour : public Texture
{
public:
	Colour m_ColourValue;

public:
	SolidColour() {};
	SolidColour(Colour _ColourValue) : m_ColourValue(_ColourValue) {};

	SolidColour(float _Red, float _Green, float _Blue) : SolidColour(Colour(_Red, _Green, _Blue)) {};

	virtual Colour Value(float u, float v, const vec3& p) const override
	{
		return m_ColourValue;
	}
};