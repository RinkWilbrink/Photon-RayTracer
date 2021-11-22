#include <precomp.h>
#include "Libraries/rtweekend/rtweekend.h"

#include "Camera/Camera.h"

#include "Libraries/Colour/Colour.h"
#include "Hittable/hittable_list.h"
#include "Sphere/Sphere.h"
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

hittable_list random_scene()
{
    hittable_list world;

    shared_ptr<Lambertian> ground_material = make_shared<Lambertian>(Colour(0.5, 0.5, 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            double choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 9.0 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    Vector3 albedo = Colour::Random() * Colour::Random();

                    sphere_material = make_shared<Lambertian>(albedo);
                    world.Add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // diffuse
                    Vector3 albedo = Colour::Random(0.5, 1.0);
                    double fuzz = random_double(0, 0.5);

                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.Add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.Add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    shared_ptr<Dielectric> material1 = make_shared<Dielectric>(1.5);
    world.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));
    
    shared_ptr<Lambertian> material2 = make_shared<Lambertian>(Colour(0.4, 0.2, 0));
    world.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    shared_ptr<Metal> material3 = make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0);
    world.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

int main()
{
    // Image
    const double aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    // World
    hittable_list world = random_scene();

    // Camera
    Point3 lookfrom(13, 2, 3);
    Point3 lookat(0, 0, 0);
    Vector3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

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