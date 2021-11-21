#include <precomp.h>
#include "Libraries/rtweekend/rtweekend.h"

#include "Camera/Camera.h"

#include "Libraries/Colour/Colour.h"
#include "Hittable/hittable_list.h"
#include "Sphere.h"
#include "Material/Material.h"

Colour ray_colour(const Ray& r, const hittable& world, int depth)
{
    hit_record rec;

    if(depth <= 0)
    {
        return Colour(0, 0, 0);
    }

    if(world.hit(r, 0.001, infinity, rec))
    {
        Ray scattered;
        Colour attentuation;
        if(rec.mat_ptr->scatter(r, rec, attentuation, scattered))
        {
            return attentuation * ray_colour(scattered, world, depth - 1);
        }
        return Colour(0, 0, 0);
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

    auto material_ground = make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Colour(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_right = make_shared<Metal>(Colour(0.8, 0.6, 0.2), 0.0);

    world.Add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.Add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.Add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    Point3 lookfrom(3, 3, 2);
    Point3 lookat(0, 0, -1);
    Vector3 vup(0, 1, 0);
    auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 2.0;
    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

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