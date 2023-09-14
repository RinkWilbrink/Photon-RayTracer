#pragma once
#include "PhotonRayTracer.h"

class PointLight
{
public:
	vec3 m_Position = vec3::Zero();
	float m_Intesity = 0.0f;
	Colour m_Colour = Colour::One();

	PointLight() {}

	/// <summary>Set the values of the Point Light</summary>
	/// <param name="_pos">The Position of the Point Light</param>
	/// <param name="_intesity">The Light Intesity of the Point Light</param>
	/// <param name="_col">The Colour of the Point Light</param>
	void SetValues(vec3 _pos, float _intesity, Colour _col)
	{
		m_Position = _pos;
		m_Intesity = _intesity;
		m_Colour = _col;
	}
};