#pragma once
#include "Libraries/Vector/Vector3.h"

#include <iostream>

void write_colour(std::ostream& out, Colour pixel_colour, int samples_per_pixel)
{
	auto r = pixel_colour.x;
	auto g = pixel_colour.y;
	auto b = pixel_colour.z;

	//
	auto scale = 1.0 / samples_per_pixel;
	r *= scale;
	g *= scale;
	b *= scale;

	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}