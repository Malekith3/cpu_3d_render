#ifndef DISPLAY_H
#define DISPLAY_H
#include <cstdint>

#include "CommonDefines.h"
#include "Vectors.h"

namespace Render
{

struct Point
{
    int x{0};
    int y{0};
    Point() = default;
    Point(float x, float y) : x(x),y(y){};
};

enum class LineRasterAlgo : uint8_t
{
    DDA,
    BRESENHAM
};

void drawGrid(ColorBufferArray& colorBuffer, uint32_t gridColor = 0xFF000000, size_t gridSpacing = 10u , size_t gridWidth = 1u);
void drawRect(ColorBufferArray& colorBuffer, int posX, int posY, size_t width, size_t height, uint32_t color);
void drawPixel(ColorBufferArray& colorBuffer, int posX, int posY, uint32_t color);
void drawRect(ColorBufferArray& colorBuffer, const Point& point1, const Point& point2, const Point& point3, size_t color = 0xFFFFFF00, LineRasterAlgo
              algoType = LineRasterAlgo::DDA);
void drawLine(ColorBufferArray& colorBuffer, const Point& startPoint, const Point& endPoint,
              LineRasterAlgo algoType = LineRasterAlgo::DDA, uint32_t  color = 0xFFFFFF00);
vect2_t<float> project(vect3_t<float>& point);
}

#endif //DISPLAY_H
