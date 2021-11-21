#pragma once
#include "Libraries/rtweekend/rtweekend.h"

class Camera
{
private:
	Point3 origin;
	Point3 lower_left_corner;
	Vector3 horizontal;
	Vector3 vertical;
	Vector3 w, u, v;
	double lens_radius;

public:
	Camera(
		Point3 lookfrom,
		Point3 lookat,
		Vector3   vup,
		double vfov, // vertical field-of-view in degrees
		double aspect_ratio,
		double aperture,
		double focus_dist
	) {
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

		lens_radius = aperture / 2;
	}

	Ray get_ray(double s, double t) const
	{
		Vector3 rd = lens_radius * random_in_unit_disk();
		Vector3 offset = u * rd.x + v * rd.y;

		return Ray(
			origin + offset,
			lower_left_corner + s * horizontal + t * vertical - origin - offset
		);
	}
};