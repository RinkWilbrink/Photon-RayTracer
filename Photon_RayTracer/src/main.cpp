#include <precomp.h>

int main(int argc, char* argv[])
{
    // Image
    const int image_width = 256;
    const int image_height = 256;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int y = image_height - 1; y >= 0; --y)
    {
        for (int x = 0; x < image_width; ++x)
        {
            auto r = double(x) / (image_width - 1);
            auto g = double(y) / (image_height - 1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);
            
            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}