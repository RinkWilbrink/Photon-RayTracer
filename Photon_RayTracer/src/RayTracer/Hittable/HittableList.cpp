#include <precomp.h>
#include "HittableList.h"

HittableList::HittableList()
{
}

HittableList::HittableList(Hittable* object)
{
    Add(object);
}

HittableList::~HittableList()
{
    objects.clear();
}

void HittableList::Clear()
{
    objects.clear();
}

void HittableList::Add(Hittable* object)
{
    objects.push_back(object);
}

bool HittableList::hit(const Ray& r, float _MinLength, float _MaxLength, HitRecord& rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    float closest_so_far = _MaxLength;

    for (const Hittable* object : objects)
    {
        if (object->hit(r, _MinLength, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool HittableList::BoundingBox(AABB& output_box) const
{
    if (objects.empty())
    {
        return false;
    }

    AABB temp_box;
    bool firstBox = true;

    for (const Hittable* object : objects)
    {
        if (!object->BoundingBox(temp_box))
        {
            return false;
        }
        output_box = firstBox ? temp_box : SurroundingBoxes(output_box, temp_box);
        firstBox = false;
    }

    return true;
}
