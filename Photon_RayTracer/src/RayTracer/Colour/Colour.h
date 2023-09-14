#pragma once

#include "Vector/Vector3.h"
#include "Template/surface.h"

Tmpl8::Pixel write_colour(Colour pixel_colour, int _SamplesPerPixel)
{
	float scale = 1.0f / _SamplesPerPixel;
	float r = sqrtf(scale * pixel_colour.x);
	float g = sqrtf(scale * pixel_colour.y);
	float b = sqrtf(scale * pixel_colour.z);

	int ir = static_cast<int>(256 * clamp(r, 0.0f, 0.999f));
	int ig = static_cast<int>(256 * clamp(g, 0.0f, 0.999f));
	int ib = static_cast<int>(256 * clamp(b, 0.0f, 0.999f));

	return ((ir << 16) + (ig << 8) + (ib));
}