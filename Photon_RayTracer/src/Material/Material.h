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
	double fuzz;

public:
	Metal(const Colour& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(const Ray& r_in, const hit_record& rec, Colour& attenuation, Ray& scattered) const override
	{
		Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};

class Dielectric : public Material
{
public:
	double ir;

private:
	static double reflectance(double cosine, double ref_idx)
	{
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

public:
	Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	virtual bool scatter(const Ray& r_in, const hit_record& rec, Colour& attentuation, Ray& scattered) const override
	{
		attentuation = Colour(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;
		Vector3 unit_direction = unit_vector(r_in.direction());

		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		
		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		Vector3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
		{
			direction = reflect(unit_direction, rec.normal);
		}
		else
		{
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}

		scattered = Ray(rec.p, direction);
		return true;
	}
};