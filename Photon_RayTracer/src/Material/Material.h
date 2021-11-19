#pragma once

#include "Libraries/rtweekend/rtweekend.h"

struct hit_record;

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const hit_record& rec, Colour& attentuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Colour albedo;

public:
	Lambertian(const Colour& a): albedo(a) {}

	virtual bool scatter(const Ray& r_in, const hit_record& rec, Colour& attenuation, Ray& scattered) const override
	{
		auto scatter_direction = rec.normal + random_unit_vector();

		if(scatter_direction.near_zero())
		{
			scatter_direction = rec.normal;
		}

		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
};

class Metal : public Material
{
public:
	Colour albedo;

public:
	Metal(const Colour& a) : albedo(a) {}

	virtual bool scatter(const Ray& r_in, const hit_record& rec, Colour& attenuation, Ray& scattered) const override
	{
		Vector3 reflected = Reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};