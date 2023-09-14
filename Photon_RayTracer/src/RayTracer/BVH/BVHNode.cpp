#include "precomp.h"

#include "BVHNode.h"
#include "AABB.h"
#include <algorithm>

inline bool BoxCompare(const Hittable* a, const Hittable* b, int axis)
{
    AABB boxA;
    AABB boxB;

    if (!a->BoundingBox(boxA) || !b->BoundingBox(boxB))
    {
        printf("No BoundingBox Found in BVH Node.\n");
    }

    return boxA.m_StartPosition[axis] < boxB.m_StartPosition[axis];
}
bool BoxXCompare(const Hittable* a, const Hittable* b) { return BoxCompare(a, b, 0); }
bool BoxYCompare(const Hittable* a, const Hittable* b) { return BoxCompare(a, b, 1); }
bool BoxZCompare(const Hittable* a, const Hittable* b) { return BoxCompare(a, b, 2); }

BVHNode::BVHNode(std::vector<Hittable*>& _Objects, uint32_t _LeftFirst, uint32_t _RightLast)
{
    std::vector<Hittable*> objects = _Objects;

    int axis = RandomInt(0, 2);
    auto comparator = (axis == 0) ? BoxXCompare : (axis == 1) ? BoxYCompare : BoxZCompare;

    uint32_t objectCount = _RightLast - _LeftFirst;

    if (objectCount == 1)
    {
        m_pLeft = m_pRight = objects[_LeftFirst];
    }
    else if (objectCount == 2)
    {
        if (comparator(objects[_LeftFirst], objects[_LeftFirst + 1]))
        {
            m_pLeft = objects[_LeftFirst];
            m_pRight = objects[_LeftFirst + 1];
        }
        else
        {
            m_pLeft = objects[_LeftFirst + 1];
            m_pRight = objects[_LeftFirst];
        }
    }
    else
    {
        std::sort(objects.begin() + _LeftFirst, objects.begin() + _RightLast, comparator);

        int mid = static_cast<int>(_LeftFirst + objectCount * 0.5f);

        m_pLeft = new BVHNode(objects, _LeftFirst, mid); // Set the Left BVH Node.
        m_pRight = new BVHNode(objects, mid, _RightLast);  // Set the Right BVH Node.
    }

    AABB boxLeft, boxRight;

    if (!m_pLeft->BoundingBox(boxLeft) || !m_pRight->BoundingBox(boxRight))
    {
        printf("No BoundingBox Found in BVH Node.\n");
    }

    // Set box to surrounding boxes.
    m_Box = SurroundingBoxes(boxLeft, boxRight);
}

bool BVHNode::hit(const Ray& r, float _MinLength, float _MaxLength, HitRecord& rec) const
{
    if (!m_Box.hit(r, _MinLength, _MaxLength))
    {
        return false;
    }

    bool hitLeft = m_pLeft->hit(r, _MinLength, _MaxLength, rec);
    bool hitRight = m_pRight->hit(r, _MinLength, hitLeft ? rec.t : _MaxLength, rec);

    return hitLeft || hitRight;
}

bool BVHNode::BoundingBox(AABB& output_box) const
{
    output_box = m_Box;
    return true;
}