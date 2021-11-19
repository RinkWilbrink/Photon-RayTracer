#include <precomp.h>
#include "Libraries/rtweekend/rtweekend.h"

#include "Libraries/Colour/Colour.h"
#include "Hittable/hittable_list.h"
#include "Sphere.h"

Colour ray_colour(const Ray& r, const hittable& world)
{
    hit_record rec;
    if(world.hit(r, 0, infinity, rec))
    {
        return 0.5 * (rec.normal + Colour(1, 1, 1));
    }
    Vector3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[])
{
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World
    hittable_list world;
    world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

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
            Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);

            Colour pixel_colour = ray_colour(r, world);
            write_colour(std::cout, pixel_colour);
        }
    }

    std::cerr << "\nDone\n";
}