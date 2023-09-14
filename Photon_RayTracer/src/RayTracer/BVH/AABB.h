#pragma once
#include "PhotonRayTracer.h"

class AABB
{
public:
    /// <summary>Start Position of the Bounding Box</summary>
    Point3 m_StartPosition;
    /// <summary>End Position of the Bounding Box</summary>
    Point3 m_EndPosition;

public:
    AABB(){}
    AABB(const Point3& _StartPosition, const Point3& _EndPosition) : m_StartPosition(_StartPosition), m_EndPosition(_EndPosition) {};

    bool hit(const Ray& _Ray, float _MinLength, float _MaxLength) const;
};

///<summary>Check the Surrounding boxes of the current Object</summary>
AABB SurroundingBoxes(AABB _Box0, AABB _Box1);