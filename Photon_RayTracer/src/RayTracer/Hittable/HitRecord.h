#pragma once
#include "RayTracer/Hittable/HitRecord.h"
#include "RayTracer/Material/Material.h"

struct HitRecord
{
public:
	Point3 m_HitPoint = Point3(0, 0, 0);
	vec3 m_Normal = vec3(0, 0, 0);
	float t = 0.0f;

	// Object Specific {insert curse word here}.
	Material* m_pMaterial = nullptr;

	Point3 m_Center = Colour::Zero();
	float m_Radius = 0.0f;

	// idk
	float u = 0.0f;
	float v = 0.0f;

	bool front_face = false;

	inline void set_face_normal(const Ray& _ray, const vec3& outward_normal)
	{
		front_face = Dot(_ray.m_Direction, outward_normal) < 0;
		m_Normal = front_face ? outward_normal : -outward_normal;
	}
};