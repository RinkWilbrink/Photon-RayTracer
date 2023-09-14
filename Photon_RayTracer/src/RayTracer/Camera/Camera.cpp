#include <precomp.h>
#include "Camera.h"

Camera::Camera(vec3 _Origin, vec3 _LookAt, vec3 _Up, float _Fov, float _AspectRatio, float _Aperture, float _FocusDistance, float _Time0, float _Time1)
{
	float theta = degrees_to_radians(_Fov);
	float h = tan(theta / 2);
	float viewportHeight = 2.0f * h;
	float viewportWidth = _AspectRatio * viewportHeight;

	m_W = vec3(_Origin - _LookAt).Normalized();
	m_U = vec3(_Up.Cross(m_W)).Normalized();
	m_V = m_W.Cross(m_U);

	//float focalLength = 1.0f;

	m_Origin = _Origin;
	m_Horizontal = _FocusDistance * viewportWidth * m_U;
	m_Vertical = _FocusDistance * viewportHeight * m_V;
	m_LowerLeftCorner = m_Origin - m_Horizontal * 0.5f - m_Vertical * 0.5f - _FocusDistance * m_W;

	M_LensRadius = _Aperture * 0.5f;

	m_Time0 = _Time0;
	m_Time1 = _Time1;
}

Ray Camera::GetRay(float s, float t) const
{
	vec3 offset = m_U + m_V;

	vec3 direction = (m_LowerLeftCorner + s * m_Horizontal + t * m_Vertical - m_Origin - offset).Normalized();
	return Ray(m_Origin + offset, direction, random_float(m_Time0, m_Time1));
}
