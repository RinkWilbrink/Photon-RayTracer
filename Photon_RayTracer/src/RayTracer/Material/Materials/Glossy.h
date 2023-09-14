#pragma once

#include "RayTracer/Material/Material.h"

class Glossy : public Material
{
public:
	vec3 albedo = vec3(0, 0, 0);

public:
	Glossy(vec3 a)
	{
		albedo = a;
	}

	virtual bool Scatter(const Ray& _Ray, const HitRecord& rec, vec3& attenuation, Ray& scattered) const override
	{

	}
};