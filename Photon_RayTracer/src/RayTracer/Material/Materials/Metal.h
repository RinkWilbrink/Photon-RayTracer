#pragma once
#include "RayTracer/Material/Material.h"


class Metal : public Material
{
public:
	Colour albedo = Colour(0, 0, 0);
	float fuzz = 0.0f;

public:
	Metal(const Colour& a, const float f)
	{
		albedo = a;
		fuzz = f;
	}

	virtual bool Scatter(const Ray& _Ray, const HitRecord& rec, Colour& attenuation, Ray& scattered) const override
	{
		Colour reflected = Reflect(UnitVector(_Ray.m_Direction), rec.m_Normal);
		scattered = Ray(rec.m_Point, reflected + fuzz * RandomInUnitSphere(), _Ray.m_Time);
		attenuation = albedo;
		return (Dot(scattered.m_Direction, rec.m_Normal) > 0);
	}
};