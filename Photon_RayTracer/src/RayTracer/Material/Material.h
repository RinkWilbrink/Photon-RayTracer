#pragma once
#include "PhotonRayTracer.h"

class Material
{
public:
	Colour m_Albedo = Colour::Zero();

	float a_Lambertian = 0.0f;
	float a_Specular = 0.0f;
	float a_Metal = 0.0f;
	float a_Dielectric = 0.0f;

	/// <summary>Set the values of a new Material.</summary>
	/// <param name="_Albedo">The Colour of the Material</param>
	/// <param name="_Lambertian">Diffuse Material</param>
	/// <param name="_Metal">Reflective Material</param>
	/// <param name="_Specular">Specular Highlight</param>
	/// <param name="_Dielectric">Refraction Index for a Dielectrics Material</param>
	Material(Colour _Albedo, float _Lambertian, float _Metal, float _Specular, float _Dielectric)
	{
		m_Albedo = _Albedo;
		a_Lambertian = _Lambertian;
		a_Metal = _Metal;
		a_Specular = _Specular;
		a_Dielectric = _Dielectric;
	};
};