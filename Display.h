#ifndef DISPLAY_H
#define DISPLAY_H
#include <cstdint>

#include "CommonDefines.h"
#include "Vectors.h"

namespace Render
{

void drawGrid(ColorBufferArray& colorBuffer, uint32_t gridColor = 0xFF000000, size_t gridSpacing = 10u , size_t gridWidth = 1u);
void drawRect(ColorBufferArray& colorBuffer, int posX, int posY, size_t width, size_t height, uint32_t color);
void drawPixel(ColorBufferArray& colorBuffer, int posX, int posY, uint32_t color);
vect2_t<float> project(vect3_t<float>& point);
}

#endif //DISPLAY_H
