#pragma once

#include "PhotonRayTracer.h"

class Camera
{
private:
	Point3 m_Origin = Point3(0, 0, 0);
	Point3 m_LowerLeftCorner = Point3(0, 0, 0);
	vec3 m_Horizontal = vec3(0, 0, 0);
	vec3 m_Vertical = vec3(0, 0, 0);

	vec3 m_W;
	vec3 m_U;
	vec3 m_V;

	float M_LensRadius = 0.0f;
	float m_Time0 = 0.0f;
	float m_Time1 = 0.0f;

public:
	Camera() {};
	Camera(Point3 _Origin, Point3 _LookAt, vec3 _Up ,float _Fov, float _AspectRatio, float _Aperture, float _FocusDistance, float _Time0 = 0.0f, float _Time1 = 0.0f);

	void MoveCamera(vec3 _AddToPosition)
	{
		m_Origin += _AddToPosition;
	}

	Ray GetRay(float u, float v) const;
};