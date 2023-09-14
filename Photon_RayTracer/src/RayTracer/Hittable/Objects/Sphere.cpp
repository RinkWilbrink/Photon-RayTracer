#include <precomp.h>
#include "Sphere.h"

inline void CalculateUV(const Point3& p, float& u, float& v)
{
    float theta = acos(-p.y);
    float aTan = atan2(-p.z, p.x + pi);

    /*
    * 1.0f / pi = 0.31847133757961783439490445859873
    * 0.5f / pi = 0.15923566878980891719745222929936
    Before:
    u = aTan * (1.0f / (2 * pi));
    v = theta * (1.0f / pi);
    */
    // After:
    float piDiv = 0.5f / pi; // = 0.1592356687898089171974522292993
    u = aTan * piDiv;
    v = theta * (piDiv * 2);
    /*
    */
}

/*
* For some of the Sphere Intersection code, I used the following link.
* https://iquilezles.org/www/articles/intersectors/intersectors.htm
*/
bool Sphere::hit(const Ray& r, float _MinLength, float _MaxLength, HitRecord& rec) const
{
    vec3 oc = r.m_Origin - m_Center;
    float b = Dot(oc, r.m_Direction);
    float c = Dot(oc, oc) - m_Radius * m_Radius;
    float discriminant = b * b - c;

    if (discriminant < 0.0f)
    {
        return false;
    }

    float sqrtd = sqrtf(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (-b - sqrtd);

    if (root < _MinLength || _MaxLength < root)
    {
        root = (-b + sqrtd);
        if (root < _MinLength || _MaxLength < root)
        {
            return false;
        }
    }

    // Set values in the HitRecord
    rec.t = root;
    rec.m_HitPoint = r.at(rec.t);
    rec.m_Center = m_Center;
    rec.m_Radius = m_Radius;
    rec.m_pMaterial = m_pMaterial;

    // Calculate normal
    vec3 outward_normal = (rec.m_HitPoint - m_Center) / m_Radius;
    rec.set_face_normal(r, outward_normal);
    float theta = acos(-outward_normal.y);
    float aTan = atan2(-outward_normal.z, outward_normal.x + pi);
    CalculateUV(outward_normal, rec.u, rec.v);

    return true;
}

bool Sphere::BoundingBox(AABB& _OutputBox) const
{
    _OutputBox = AABB(
        m_Center - vec3(m_Radius, m_Radius, m_Radius),
        m_Center + vec3(m_Radius, m_Radius, m_Radius));
    return true;
}

void Sphere::GetSphereUV(const Point3& p, float& u, float& v)
{
    CalculateUV(p, u, v);
}