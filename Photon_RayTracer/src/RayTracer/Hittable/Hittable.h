#pragma once
#include "PhotonRayTracer.h"
#include "HitRecord.h"
#include "RayTracer/BVH/AABB.h"

class Material;

class Hittable
{
public:
    virtual bool hit(const Ray& r, float _MinLength, float _MaxLength, HitRecord& rec) const = 0;
    virtual bool BoundingBox(AABB& _OutputBox) const = 0;
};