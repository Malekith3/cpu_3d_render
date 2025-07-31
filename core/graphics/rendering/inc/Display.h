#ifndef DISPLAY_H
#define DISPLAY_H
#include <cstdint>

#include "common/inc/Colors.h"
#include "graphics/textures/inc/Textures.h"

#include "common/inc/CommonDefines.h"
#include "common/inc/Vectors.hpp"


struct Triangle;

namespace Render
{

struct Point
{
    int32_t x{0};
    int32_t y{0};
    Point() = default;
    Point(const float x, const float y) : x(static_cast<int32_t>(x)),y(static_cast<int32_t>(y)){};
    Point(int32_t x, int32_t y) : x(x), y(y) {}
};

enum class LineRasterAlgo : uint8_t
{
    DDA,
    BRESENHAM
};

void drawGrid(ColorBufferArray& colorBuffer, uint32_t gridColor = toColorValue(Colors::BLACK), size_t gridSpacing = 10u , size_t gridWidth = 1u);
void drawRect(ColorBufferArray& colorBuffer, int posX, int posY, size_t width, size_t height, uint32_t color);
void drawPixel(ColorBufferArray& colorBuffer, int posX, int posY, uint32_t color);
void drawTriangle(ColorBufferArray& colorBuffer, const Point& point1, const Point& point2, const Point& point3, size_t color = toColorValue(Colors::WHITE), LineRasterAlgo
              algoType = LineRasterAlgo::DDA);
void drawLine(ColorBufferArray& colorBuffer, const Point& startPoint, const Point& endPoint,
              LineRasterAlgo algoType = LineRasterAlgo::DDA, uint32_t  color = toColorValue(Colors::WHITE));
vect2_t<float> projectNonMatrix(const vect3_t<float>& point);
void drawFilledTriangleFlatBottom(ColorBufferArray& colorBuffer, const Triangle& triangle, size_t color = toColorValue(Colors::WHITE));
void drawTexturedTriangle(ColorBufferArray& colorBuffer, const Triangle& triangle, Texture2dArray& texture);
}

#endif //DISPLAY_H
