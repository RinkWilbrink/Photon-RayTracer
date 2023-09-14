#pragma once

#include "PhotonRayTracer.h"
#include "RayTracer/Hittable/HittableList.h"

class BVHNode : public Hittable
{
public:
    Hittable* m_pLeft = nullptr;
    Hittable* m_pRight = nullptr;
    AABB m_Box;

public:
    BVHNode(HittableList& _List) : BVHNode(_List.objects, 0, _List.objects.size()) {};
    BVHNode(std::vector<Hittable*>& _Objects, uint32_t _LeftFirst, uint32_t _RightLast);

    virtual bool hit(const Ray& r, float _MinLength, float _MaxLength, HitRecord& rec) const override;
    virtual bool BoundingBox(AABB& output_box) const override;
};

inline bool BoxCompare(const Hittable* a, const Hittable* b, int axis);
bool BoxXCompare(const Hittable* a, const Hittable* b);
bool BoxYCompare(const Hittable* a, const Hittable* b);
bool BoxZCompare(const Hittable* a, const Hittable* b);