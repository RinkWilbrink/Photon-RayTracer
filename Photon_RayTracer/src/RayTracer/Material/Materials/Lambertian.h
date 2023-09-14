#pragma once
#include "RayTracer/Material/Material.h"
#include "RayTracer/Material/Materials/SolidColour.h"

class Lambertian : public Material
{
public:
	Texture* albedo = nullptr;

public:
	Lambertian(const Colour& a)
	{
		albedo = new SolidColour(a);
	}
	Lambertian(Texture* _Texture)
	{
		albedo = _Texture;
	}

	virtual bool Scatter(const Ray& _Ray, const HitRecord& rec, Colour& attenuation, Ray& scattered) const override
	{
		auto scatter_direction = rec.m_Normal + RandomUnitVector();

		// Catch degenerate scatter direction
		if (scatter_direction.NearZero())
		{
			scatter_direction = rec.m_Normal;
		}

		scattered = Ray(rec.m_Point, scatter_direction, _Ray.m_Time);
		attenuation = albedo->Value(rec.u, rec.v, rec.m_Point);
		return true;
	}
};