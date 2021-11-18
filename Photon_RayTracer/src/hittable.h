#pragma once

#include "Libraries/Ray/Ray.h"

struct hit_record
{
	Point3 p;
	Vector3 normal;
	double t;
};

class hittable
{
public:
	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};