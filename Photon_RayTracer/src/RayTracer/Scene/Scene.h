#pragma once
#include "RayTracer/Hittable/HittableList.h"

class Scene
{
public:
    HittableList CreateRandomScene(bool addBVH, int _AxisCount);
    HittableList DielectricUpClose(bool addBVH, bool ExtraSpheres);

    HittableList ColouredMaterialsShowCase(bool addBVH);

private:
    /// <summary>Return a BVH with all objects inside, propperly sorted.</summary>
    /// <param name="newWorld">The HittableList with all the objects in the scene.</param>
    HittableList AddBVH(HittableList& newWorld);
};