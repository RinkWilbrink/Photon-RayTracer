#include <precomp.h>
#include "Libraries/rtweekend/rtweekend.h"

#include "Camera/Camera.h"

#include "Libraries/Colour/Colour.h"
#include "Hittable/hittable_list.h"
#include "Sphere.h"

Colour ray_colour(const Ray& r, const hittable& world, int depth)
{
    hit_record rec;

    if(depth <= 0)
    {
        return Colour(0, 0, 0);
    }

    if(world.hit(r, 0.001, infinity, rec))
    {
        Point3 target = rec.p + rec.normal + random_in_hemisphere(rec.normal);
        return 0.5 * ray_colour(Ray(rec.p, target - rec.p), world, depth - 1);
    }

    Vector3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 0.7, 1.0);
}

int main()
{
    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    hittable_list world;
    world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera
    Camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int y = image_height - 1; y >= 0; --y) // j
    {
        std::cerr << "\r" << y << " of " << image_height - 1 << " Scan Lines Remaining!" << ' ' << std::flush;

        for (int x = 0; x < image_width; ++x) // i
        {
            Colour pixel_colour(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (x + random_double()) / (image_width - 1);
                auto v = (y + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_colour += ray_colour(r, world, max_depth);
            }
            write_colour(std::cout, pixel_colour, samples_per_pixel);
        }
    }

    std::cerr << "\nDone\n";
}