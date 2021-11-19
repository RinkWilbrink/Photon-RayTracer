#pragma once

#include <cmath>
#include <iostream>

class Vector3
{
public:
    Vector3() {}
    Vector3(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }

    Vector3 operator-() const { return Vector3(-x, -y, -z); }
    //double operator[](int i) const { return e[i]; }
    //double& operator[](int i) { return e[i]; }

    Vector3& operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3& operator*=(const double t)
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vector3& operator/=(const double t)
    {
        return *this *= 1/t;
    }

    double length() const
    {
        return sqrt(length_sqr());
    }

    double length_sqr() const
    {
        return (x * x) + (y * y) + (z * z);
    }

    inline static Vector3 Random()
    {
        return Vector3(random_double(), random_double(), random_double());
    }
    inline static Vector3 Random(double min, double max)
    {
        return Vector3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    bool near_zero() const
    {
        const double s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }

public:
    //double e[3];
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

using Colour = Vector3;
using Point3 = Vector3;

// Vector3 Utility Functions

inline std::ostream& operator<<(std::ostream &out, const Vector3 &v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline Vector3 operator+(const Vector3& u, const Vector3& v)
{
    return Vector3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v)
{
    return Vector3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v)
{
    return Vector3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vector3 operator*(double t, const Vector3& v)
{
    return Vector3(t * v.x, t * v.y, t * v.z);
}

inline Vector3 operator*(const Vector3& v, double t)
{
    return t * v;
}

inline Vector3 operator/(Vector3 v, double t)
{
    return (1 / t) * v;
}

inline double dot(const Vector3& u, const Vector3& v)
{
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

inline Vector3 cross(const Vector3& u, const Vector3& v)
{
    return Vector3((u.y * v.z) - (u.z, v.y),
                   (u.z * v.x) - (u.x, v.z),
                   (u.x * v.y) - (u.y, v.z));
}

inline Vector3 unit_vector(Vector3 v)
{
    return v / v.length();
}

Vector3 random_in_unit_sphere()
{
    while(true)
    {
        Vector3 p = Vector3::Random(-1, 1);
        if(p.length_sqr() >= 1)
        {
            continue;
        }
        return p;
    }
}

Vector3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

Vector3 random_in_hemisphere(const Vector3& normal)
{
    Vector3 in_unit_sphere = random_in_unit_sphere();
    if(dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
    {
        return in_unit_sphere;
    }
    return -in_unit_sphere;
}

Vector3 Reflect(const Vector3& v, const Vector3& n)
{
    return v - 2 * dot(v, n) * n;
}