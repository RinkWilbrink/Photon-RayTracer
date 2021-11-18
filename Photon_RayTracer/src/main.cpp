#include <precomp.h>
#include "Vector/Vector3.h"
#include "Colour/Colour.h"
#include "Ray/Ray.h"

double hit_sphere(const Point3& center, double radius, const Ray& r)
{
    Vector3 oc = r.origin();
    auto a = r.direction().length_sqr();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_sqr() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if(discriminant >= 0)
    {
        return (-half_b - sqrt(discriminant)) / a;
    }
    return -1.0;
}

Colour ray_colour(const Ray& r)
{
    auto t = hit_sphere(Point3(0, 0, -1), 0.5, r);
    if(t > 0.0)
    {
        Vector3 N = unit_vector(r.at(t) - Vector3(0, 0, -1));
        return 0.5 * Colour(N.x + 1, N.y + 1, N.z + 1);
    }
    Vector3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[])
{
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = Point3(0, 0, 0);
    auto horizontal = Vector3(viewport_width, 0, 0);
    auto vertical = Vector3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int y = image_height - 1; y >= 0; --y) // j
    {
        std::cerr << "\rScanlines Remaining: " << y << ' ' << std::flush;

        for (int x = 0; x < image_width; ++x) // i
        {
            auto u = double(x) / (image_width - 1);
            auto v = double(y) / (image_height - 1);
            Ray ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);

            Colour pixel_colour = ray_colour(ray);
            write_colour(std::cout, pixel_colour);
        }
    }

    std::cerr << "\nDone\n";
}