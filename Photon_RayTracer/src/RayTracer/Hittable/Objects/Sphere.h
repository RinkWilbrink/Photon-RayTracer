#pragma once
#include "RayTracer/Hittable/Hittable.h"

class Sphere : public Hittable
{
public:
    Point3 m_Center = Point3(0, 0, 0);
    float m_Radius = 0.0f;
    Material* m_pMaterial = nullptr;

public:
    Sphere() {}
    Sphere(Point3 _center, float _radius, Material* _pMaterial) : m_Center(_center), m_Radius(_radius), m_pMaterial(_pMaterial) {}

    virtual bool hit(const Ray& r, float _MinLength, float _MaxLength, HitRecord& rec) const override;
    virtual bool BoundingBox(AABB& _OutputBox) const override;

private:
    static void GetSphereUV(const Point3& p, float& u, float& v);
};