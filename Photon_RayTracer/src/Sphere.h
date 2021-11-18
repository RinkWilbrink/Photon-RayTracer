#pragma once

#include "hittable.h"
#include "Libraries/Vector/Vector3.h"

class Sphere : public hittable
{
public:
	Point3 center;
	double radius;

public:
	Sphere(){}
	Sphere(Point3 cen, double r) : center(cen), radius(r) {};

	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool Sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    Vector3 oc = r.origin() - center;
    auto a = r.direction().length_sqr();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_sqr() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if(discriminant < 0)
    {
        return false;
    }

    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.

    auto root = (-half_b - sqrtd) / a;
    if(root < t_min || root > t_max)
    {
        root = (-half_b + sqrtd) / a;
        if(root < t_min || root > t_max)
        {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

	return true;
}