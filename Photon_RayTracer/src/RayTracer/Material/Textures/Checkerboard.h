#pragma once
#include "RayTracer/Material/Texture.h"

class Checkerboard : public Texture
{
public:
	Texture* m_Even = nullptr;
	Texture* m_Odd = nullptr;

public:
	Checkerboard()
	{
	}

	Checkerboard(Texture* _Even, Texture* _Odd)
	{
		m_Even = _Even;
		m_Odd = _Odd;
	}

	Checkerboard(Colour _Even, Colour _Odd)
	{
		m_Even = new SolidColour(_Even);
		m_Odd = new SolidColour(_Odd);
	}

	virtual Colour Value(float u, float v, const Point3& p) const override
	{
		float sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
		if (sines < 0)
		{
			return m_Odd->Value(u, v, p);
		}
		return m_Even->Value(u, v, p);
	}
};