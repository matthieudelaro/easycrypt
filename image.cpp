#include "image.h"

Image::Image(unsigned int width, unsigned int height)
{
    pixels.resize(height);
    for(unsigned int i = 0; i < height; i++)
    {
        pixels[i].resize(width);
        for(unsigned int j = 0; j < width; j++)
            pixels[i][j].resize(4);
    }
}
