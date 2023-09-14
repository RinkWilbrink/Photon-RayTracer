#pragma once
#include "PhotonRayTracer.h"
#include "RayTracer/Material/Texture.h"

/*
 * Chapter 5.6
*/

class PerlinNoise
{
private:
    static const int m_PointCount = 256;

    vec3* m_RandomVector = nullptr;
    float* m_RandomFloat = nullptr;
    int* perm_x = nullptr;
    int* perm_y = nullptr;
    int* perm_z = nullptr;

public:
    PerlinNoise()
    {
        m_RandomVector = new vec3[m_PointCount];
        for (int i = 0; i < m_PointCount; i++)
        {
            m_RandomVector[i] = UnitVector(Vector::Random(-1, 1));
        }

        m_RandomFloat = new float[m_PointCount];
        for (int i = 0; i < m_PointCount; ++i)
        {
            m_RandomFloat[i] = random_float();
        }

        perm_x = PerlinNoiseGenerator();
        perm_y = PerlinNoiseGenerator();
        perm_z = PerlinNoiseGenerator();
    }

    ~PerlinNoise()
    {
        delete[] m_RandomFloat;
        delete[] m_RandomVector;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    /// <summary>Return an Interpolated Noise value based on position.</summary>
    /// <param name="p">Position</param>
    float Noise(const vec3& p) const
    {
        float u = p.x - floor(p.x);
        float v = p.y - floor(p.y);
        float w = p.z - floor(p.z);

        u = u * u*(3 - 2 * u);
        v = v * v*(3 - 2 * v);
        w = w * w*(3 - 2 * w);

        int i = static_cast<int>(floor(p.x));
        int j = static_cast<int>(floor(p.y));
        int k = static_cast<int>(floor(p.z));
        vec3 c[2][2][2];

        for (int di = 0; di < 2; di++)
        {
            for (int dj = 0; dj < 2; dj++)
            {
                for (int dk = 0; dk < 2; dk++)
                {
                    c[di][dj][dk] = m_RandomVector[
                        perm_x[(i + di) & 255] ^
                        perm_y[(j + dj) & 255] ^
                        perm_z[(k + dk) & 255] ];
                }
            }
        }

        return PerlinInterpolation(c, u, v, w);
    }

    float Turbulent(const vec3& p, int depth = 7) const
    {
        float accumulate = 0.0f;
        vec3 RT_TimerRunning = p;
        float weight = 1.0f;

        for (int i = 0; i < depth; i++)
        {
            accumulate += weight * Noise(RT_TimerRunning);
            weight *= 0.5f;
            RT_TimerRunning *= 2;
        }

        return fabsf(accumulate);
    }

private:
    static int* PerlinNoiseGenerator()
    {
        auto p = new int[m_PointCount];

        for (int i = 0; i < PerlinNoise::m_PointCount; i++)
            p[i] = i;

        Permutation(p, m_PointCount);

        return p;
    }

    static void Permutation(int* p, int n)
    {
        for (int i = n - 1; i > 0; i--)
        {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    // Interpolate Perlin noise based on a Vector3, bit more complicated then TrilinearInterpolation().
    static float PerlinInterpolation(vec3 c[2][2][2], float u, float v, float w)
    {
        float uu = u * u * (3 - 2 * u);
        float vv = v * v * (3 - 2 * v);
        float ww = w * w * (3 - 2 * w);
        float accumulate = 0.0f;

        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                {
                    vec3 weight_v(u - i, v - j, w - k);
                    accumulate += (i * uu + (1 - i) * (1 - uu))
                        * (j * vv + (1 - j) * (1 - vv))
                        * (k * ww + (1 - k) * (1 - ww))
                        * c[i][j][k].Dot(weight_v);
                }

        return accumulate;
    }

    // Trilinearelly interpolate Perlin noise for a smoother Perlin noise texture.
    static float TrilinearInterpolation(float c[2][2][2], float u, float v, float w)
    {
        auto accumulate = 0.0f;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    accumulate +=
                        (i * u + (1 - i) * (1 - u)) *
                        (j * v + (1 - j) * (1 - v)) *
                        (k * w + (1 - k) * (1 - w)) * c[i][j][k];
                }
            }
        }

        return accumulate;
    }
};