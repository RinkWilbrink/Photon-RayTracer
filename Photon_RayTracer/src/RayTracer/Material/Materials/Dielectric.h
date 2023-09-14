#pragma once
#include "RayTracer/Material/Material.h"

class Dielectric : public Material
{
public:
	float ir;

public:
    Dielectric(float index_of_refraction)
    {
        ir = index_of_refraction;
    }

	virtual bool Scatter(const Ray& _Ray, const HitRecord& _Rec, Colour& _Attenuation, Ray& _Scattered) const override
    {
        _Attenuation = Colour(1.0f, 1.0f, 1.0f);
        float refraction_ratio = _Rec.front_face ? (1.0f / ir) : ir;

        vec3 unit_direction = UnitVector(_Ray.m_Direction);
        float cos_theta = fmin(Dot(-unit_direction, _Rec.m_Normal), 1.0f);
        float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
        vec3 direction;
        if (cannot_refract || Reflectance(cos_theta, refraction_ratio) > random_float())
        {
            direction = Reflect(unit_direction, _Rec.m_Normal);
        }
        else
        {
            direction = Refract(unit_direction, _Rec.m_Normal, refraction_ratio);
        }

        _Scattered = Ray(_Rec.m_Point, direction, _Ray.m_Time);
        return true;
    }

private:
    static float Reflectance(float _CoSine, float _RefIdx)
    {
        // Use Schlick's approximation for Reflectance.
        float r0 = (1 - _RefIdx) / (1 + _RefIdx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - _CoSine), 5);
    }
};