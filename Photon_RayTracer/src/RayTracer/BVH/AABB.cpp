#include "precomp.h"
#include "AABB.h"

bool AABB::hit(const Ray& _Ray, float _MinLength, float _MaxLength) const
{
    /* Ray/Box Intersection code by Andrew Kensler 
    */
    for (int a = 0; a < 3; a++)
    {
        float invD = 1.0f / _Ray.m_Direction[a]; 
        // instead of dividind t0 and t1, we can multiply by invD because for both cases invD would be the same value.
        float t0 = (m_StartPosition[a] - _Ray.m_Origin[a]) * invD;
        float t1 = (m_EndPosition[a] - _Ray.m_Origin[a]) * invD;

        if (invD < 0.0f)
        {
            std::swap(t0, t1);
        }

        _MinLength = t0 > _MinLength ? t0 : _MinLength;
        _MaxLength = t1 < _MaxLength ? t1 : _MaxLength;

        if (_MaxLength <= _MinLength)
        {
            return false;
        }
    }
    return true;
}

AABB SurroundingBoxes(AABB _Box0, AABB _Box1)
{
    Point3 small(
        fmin(_Box0.m_StartPosition.x, _Box1.m_StartPosition.x),
        fmin(_Box0.m_StartPosition.y, _Box1.m_StartPosition.y),
        fmin(_Box0.m_StartPosition.z, _Box1.m_StartPosition.z));
    Point3 big(
        fmax(_Box0.m_EndPosition.x, _Box1.m_EndPosition.x),
        fmax(_Box0.m_EndPosition.y, _Box1.m_EndPosition.y),
        fmax(_Box0.m_EndPosition.z, _Box1.m_EndPosition.z));

    return AABB(small, big);
}