#pragma once

#include "Vector/Vector3.h"

class Ray
{
public:
	Point3 m_Origin;
	vec3 m_Direction;
	float m_Time = 0.0f;

public:
	Ray() {}
	Ray(const vec3& _origin, const vec3& _direction, float _Time = 0.0f) :
		m_Origin(_origin), m_Direction(_direction), m_Time(_Time) {}

	vec3 at(float t) const
	{
		return m_Origin + t * m_Direction;
	}
};