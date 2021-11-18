#pragma once
#include "Libraries/Vector/Vector3.h"

#include <iostream>

void write_colour(std::ostream& out, Colour pixel_colour)
{
	out << static_cast<int>(255.999 * pixel_colour.x) << ' '
		<< static_cast<int>(255.999 * pixel_colour.y) << ' '
		<< static_cast<int>(255.999 * pixel_colour.z) << '\n';
}