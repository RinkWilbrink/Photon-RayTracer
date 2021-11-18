#pragma once

#include <cmath>
#include <iostream>

class Vector3
{
public:
    Vector3() {}
    Vector3(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }

    double X() const { return x; }
    double Y() const { return y; }
    double Z() const { return z; }

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

public:
    //double e[3];
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

using colour = Vector3;
using point = Vector3;

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

inline Vector3 operator*(double t, Vector3& v)
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