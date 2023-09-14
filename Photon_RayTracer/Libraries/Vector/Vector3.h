#pragma once

#include <cmath>
#include <iostream>
#include "Math/Math.h"

using namespace Tmpl8;

using Colour = Tmpl8::vec3;
using Point3 = Tmpl8::vec3;

class Vector
{
public:
    inline static Tmpl8::vec3 Random()
    {
        return Tmpl8::vec3(random_float(), random_float(), random_float());
    }
    inline static Tmpl8::vec3 Random(float min, float max)
    {
        return Tmpl8::vec3(random_float(min, max), random_float(min, max), random_float(min, max));
    }
};

/*
*/
inline Tmpl8::vec3 UnitVector(vec3 v)
{
    float length = 1.0f / v.Length();
    return Tmpl8::vec3(v.x * length, v.y * length, v.z * length);
}

inline Tmpl8::vec3 RandomInUnitSphere()
{
    while (true)
    {
        Tmpl8::vec3 point = Vector::Random(-1, 1);
        if (point.sqrLenght() >= 1)
        {
            continue;
        }
        return point;
    }
}

inline Tmpl8::vec3 RandomUnitVector()
{
    return RandomInUnitSphere().Normalized();
}

inline Tmpl8::vec3 RandomInHemisphere(const Tmpl8::vec3& normal)
{
    Tmpl8::vec3 in_unit_sphere = RandomInUnitSphere();
    if (in_unit_sphere.Dot(normal) > 0.0f) // In the same hemisphere as the normal
    {
        return in_unit_sphere;
    }
    return -in_unit_sphere;
}

inline Tmpl8::vec3 Reflect(const Tmpl8::vec3& v, const Tmpl8::vec3& n)
{
    return v - 2 * v.Dot(n) * n;
    //return v - 2 * Dot(v, n) * n;
}

inline Tmpl8::vec3 Refract(const Tmpl8::vec3& uv, const Tmpl8::vec3& n, float etai_over_etat)
{
    float cos_theta = fmin(n.Dot(-uv), 1.0f);
    Tmpl8::vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Tmpl8::vec3 r_out_parallel = -sqrtf(fabsf(1.0f - r_out_perp.sqrLenght())) * n;
    return r_out_perp + r_out_parallel;
}

// ------------------------------------------------------------

class Vector3Int
{
public:
    int x = 0;
    int y = 0;
    int z = 0;

public:


private:
};