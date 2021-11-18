#include <precomp.h>
#include "Vector/Vector3.h"
#include "Colour/Colour.h"

int main(int argc, char* argv[])
{
    // Image
    const int image_width = 256;
    const int image_height = 256;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int y = image_height - 1; y >= 0; --y)
    {
        std::cerr << "\rScanlines Remaining: " << y << ' ' << std::flush;

        for (int x = 0; x < image_width; ++x)
        {
            Colour pixel_colour(double(x) / (image_width - 1), double(y) / (image_height - 1), 0.25);
            write_colour(std::cout, pixel_colour);
        }
    }

    std::cerr << "\nDone\n";
}