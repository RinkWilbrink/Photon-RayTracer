#pragma once
#include <memory>
#include <vector>

#include "Hittable.h"
#include "RayTracer/LightSource/LightSource.h"

struct HitRecord;

class HittableList : public Hittable
{
public:
    HittableList();
    HittableList(Hittable* object);
    ~HittableList();

    void Clear();
    void Add(Hittable* object);

    virtual bool hit(const Ray& r, float _MinLength, float _MaxLength, HitRecord& rec) const override;
    virtual bool BoundingBox(AABB& output_box) const override;

    /// <summary>Set the camera values for the current scene</summary>
    /// <param name="_Origin">Starting Position of the Camera</param>
    /// <param name="_LookAt">Look at position for the Camera</param>
    /// <param name="_Up">Up direction for the Camera, can be used to make the camera upside down or not.</param>
    /// <param name="_Fov">FOV of the Camera</param>
    /// <param name="_AspectRatio">Aspect Ratio of the Camera.</param>
    /// <param name="_FocusDistance">Focus Distance of the Camera, for Depth of Field</param>
    /// <param name="_Aperture">Aperture of the Camera.</param>
    void SetCameraValues(vec3 _Origin, vec3 _LookAt, vec3 _Up, float _Fov, float _AspectRatio, float _FocusDistance, float _Aperture)
    {
        m_CamPos = _Origin;
        m_CamLookAt = _LookAt;
        m_CamFOV = _Fov;
        m_CamUp = _Up;
        m_CamDistanceToFocus = _FocusDistance;
        m_CamAperture = _Aperture;
    }

    void SetAthmosSphereValues(Colour _AtmosphereColour, Colour _AtmosphereBrightness)
    {
        m_LowerSkyColour = _AtmosphereColour;
        m_UpperSkyColour = _AtmosphereBrightness;
    }

public:
    uint32_t ObjectCount = 0;
    std::vector<Hittable*> objects = std::vector<Hittable*>();

    // Scene specific values and Settings
    PointLight pointLight;

    Point3 m_CamPos = Point3::Zero();
    Point3 m_CamLookAt = Point3::Zero();
    float m_CamFOV = 45.0f;
    vec3 m_CamUp = vec3(0, 1, 0);
    float m_CamDistanceToFocus = 10.0f;
    float m_CamAperture = 0.0f;

    Colour m_LowerSkyColour = Colour(0.5f, 0.7f, 1.0f); // 0.5f, 0.7f, 1.0f
    Colour m_UpperSkyColour = Colour(1.0f, 1.0f, 1.0f);
};