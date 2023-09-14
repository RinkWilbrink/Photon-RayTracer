#include "precomp.h"
#include "Scene.h"

#include "RayTracer/BVH/BVHNode.h"

// Materials
#include "RayTracer/Material/Material.h"

// Objects
#include "RayTracer/Hittable/Objects/Sphere.h"

HittableList Scene::AddBVH(HittableList& newWorld)
{
    HittableList bvhWorld;
    bvhWorld.ObjectCount = newWorld.ObjectCount;
    bvhWorld.pointLight = newWorld.pointLight;
    bvhWorld.Add(new BVHNode(newWorld));

    return bvhWorld;
}

HittableList Scene::CreateRandomScene(bool addBVH, int _AxisCount)
{
    HittableList newWorld;
    newWorld.pointLight.SetValues(vec3(0, 20, 2), 1.0f, Colour(1.0f, 1.0f, 1.0f));

    Material* groundMaterial = new Material(Colour(0.8f, 0.8f, 0.8f), 1.0f, 0.0f, 0.0f, 0.0f);
    newWorld.Add(new Sphere(Point3(0, -1000, 0), 1000, groundMaterial));
    newWorld.ObjectCount += 1;

    for (int a = 0; a < _AxisCount; a++)
    {
        for (int b = 0; b < _AxisCount; b++)
        {
            float chooseMat = random_float();
            Point3 center = Point3(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

            if ((center - Point3(4, 0.2f, 0)).Length() > 0.9f)
            {
                Material* sphere_material;

                if (chooseMat < 0.8f)
                {
                    // diffuse
                    Colour albedo = Vector::Random() * Vector::Random();
                    sphere_material = new Material(albedo, 1.0f, 0.0f, 0.0f, 0.0f);
                    vec3 center2 = center + vec3(0, random_float(0, 0.5f), 0);
                    newWorld.Add(new Sphere(center, 0.2f, sphere_material));
                }
                else if (chooseMat < 0.95f)
                {
                    // metal
                    Colour albedo = Vector::Random(0.5f, 1);
                    float fuzz = random_float(0, 0.5f);
                    sphere_material = new Material(albedo, 0.9f - fuzz, fuzz, 1.0f, 0.0f);
                    newWorld.Add(new Sphere(center, 0.2f, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = new Material(Colour::One(), 0.0f, 0.0f, 0.0f, 1.5f);
                    newWorld.Add(new Sphere(center, 0.2f, sphere_material));
                }

                newWorld.ObjectCount += 1;
            }
        }
    }

    if (addBVH == true)
    {
        return AddBVH(newWorld);
    }
    return newWorld;
}

HittableList Scene::DielectricUpClose(bool addBVH, bool ExtraSpheres)
{
    HittableList newWorld;

    newWorld.pointLight.SetValues(vec3(0, 20, 2), 0.7f, Colour(1.0f, 1.0f, 1.0f));

    if (ExtraSpheres)
    {
        Material* lambert = new Material(Colour(0.4f, 0.4f, 0.9f), 1.0f, 0.0f, 0.0f, 0.0f);
        newWorld.Add(new Sphere(Point3(0, -2000, 0), 2000, lambert));
        Material* metal = new Material(Colour(1.0f, 0.5f, 0.5f), 0.0f, 1.0f, 1.0f, 0.0f);
        newWorld.Add(new Sphere(Point3(2.0f, 1, 4.0f), 0.5f, metal));
    }

    Material* glass = new Material(Colour(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.0f, 1.5f);
    newWorld.Add(new Sphere(Point3(0, 1, 0), 1, glass));

    // Check if allowed to add a BVH and if yes, Add a BVH.
    if (addBVH == true)
    {
        return AddBVH(newWorld);
    }
    return newWorld;
}

HittableList Scene::ColouredMaterialsShowCase(bool addBVH)
{
    HittableList newWorld;
    newWorld.pointLight.SetValues(vec3(0, 20, 2), 1.0f, Colour(1.0f, 1.0f, 1.0f));

    // Floor
    Material* groundMaterial = new Material(Colour(0.9f, 0.9f, 0.7f), 1.0f, 0.0f, 1.0f, 0.0f);
    newWorld.Add(new Sphere(Point3(0, -1000, 0), 1000, groundMaterial));

    Material* glass = new Material(Colour::One(), 0.0f, 0.0f, 0.0f, 1.5f);
    Material* glass2 = new Material(Colour(0.1f, 0.9f, 0.1f), 1.0f, 0.0f, 0.0f, 1.5f);
    newWorld.Add(new Sphere(Point3(-4, 1, 0), 1.0f, glass));
    newWorld.Add(new Sphere(Point3(4, 3.5f, 0), 1.0f, glass2));

    Material* lambert = new Material(Colour(0.7f, 0.3f, 0.4f), 1.0f, 0.0f, 0.0f, 0.0f);
    newWorld.Add(new Sphere(Point3(0, 1, 0), 1.0f, lambert));

    Material* metal = new Material(Colour::One(), 0.0f, 1.0f, 1.0f, 0.0f);
    Material* purpleMetal = new Material(Colour(1.0f, 0.1f, 1.0f), 2.0f, 1.0f, 1.0f, 0.0f);
    newWorld.Add(new Sphere(Point3(4, 1, 0), 1.0f, metal));
    newWorld.Add(new Sphere(Point3(-4, 3.5f, 0), 1.0f, purpleMetal));

    if (addBVH == true)
    {
        return AddBVH(newWorld);
    }
    return newWorld;
}
