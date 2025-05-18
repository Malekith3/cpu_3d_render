#include "Display.h"

#include <algorithm>
#include <cmath>
#include <CommonDefines.h>
#include <span>
#include <Vectors.hpp>
#include <Math3D.h>


#include "glm/mat4x4.hpp"

namespace
{
    float fovFactor = 600.0f;
    constexpr float EPSILON = std::numeric_limits<float>::epsilon(); // Machine epsilon for float precision
}


namespace Render
{

void drawGrid(ColorBufferArray& colorBuffer, uint32_t gridColor, size_t gridSpacing, size_t gridWidth)
{
    for (size_t i{0u}; i < colorBuffer.size(); i++)
    {
        size_t columnIndex = i % WINDOW_WIDTH;
        size_t rowIndex = i / WINDOW_WIDTH;

        if (columnIndex % gridSpacing < gridWidth || rowIndex % gridSpacing < gridWidth)
        {
            colorBuffer[i] = gridColor;
        }
    }
}

void drawRect(ColorBufferArray& colorBuffer, const int posX, const int posY, const size_t width, const size_t height, const uint32_t color)
{
    size_t numberOfRow = posY;
    while (numberOfRow < posY + height)
    {
        const size_t startIdx = numberOfRow * WINDOW_WIDTH + posX;
        auto row = std::span(colorBuffer.data() + startIdx, width);
        std::ranges::fill(row, color);
        numberOfRow++;
    }
}

void drawPixel(ColorBufferArray& colorBuffer, const int posX, const int posY, const uint32_t color)
{
    if (posX == 0 || posY == 0)
    {
        return;
    }

    if (const size_t index{WINDOW_WIDTH*posY + posX};
        index < colorBuffer.size())
    {
        colorBuffer[index] = color;
    }
}

void drawTriangle(ColorBufferArray& colorBuffer,
              const Point& point1, const Point& point2, const Point& point3,
              size_t color, LineRasterAlgo algoType)
{
    drawLine(colorBuffer,point1,point2, algoType, color);
    drawLine(colorBuffer,point2,point3, algoType, color);
    drawLine(colorBuffer,point3,point1, algoType, color);

}

vect2_t<float> projectNonMatrix(const vect3_t<float>& point)
{
    return fovFactor * vect2_t<float>{point.x / point.z ,point.y / point.z};
}

void drawLine(ColorBufferArray& colorBuffer, const Point& startPoint, const Point& endPoint, const LineRasterAlgo algoType, uint32_t color)
{
    auto drawWithDDAAlgo = [&]()
    {
        int deltaX{endPoint.x - startPoint.x};
        int deltaY{endPoint.y - startPoint.y};

        int steps = std::max(abs(deltaX),abs(deltaY));

        float incrX = deltaX / static_cast<float>(steps);
        float incrY = deltaY / static_cast<float>(steps);

        float currenX = startPoint.x;
        float currentY = startPoint.y;

        for(auto i{0u}; i < steps; i++)
        {
            drawPixel(colorBuffer,std::round(currenX),std::round(currentY),color);
            currenX+=incrX;
            currentY+=incrY;
        }
    };

    auto drawWithBresenhamAlgo = [&]()
    {
        int deltaX{endPoint.x - startPoint.x};
        const int8_t  ix((deltaX > 0) - (deltaX < 0));
        deltaX = std::abs(deltaX) << 1;

        int deltaY{endPoint.y - startPoint.y};
        const int8_t  iy((deltaY > 0) - (deltaY < 0));
        deltaY = std::abs(deltaY) << 1;

        Point newRasterPoint{startPoint};
        drawPixel(colorBuffer,newRasterPoint.x,newRasterPoint.y, color);


        if (deltaX >= deltaY)
        {
            // error may go below zero
            int error(deltaY - (deltaX >> 1));

            while (newRasterPoint.x != endPoint.x)
            {
                // reduce error while taking into account the corner case of error == 0
                if ((error > 0) || (!error && (ix > 0)))
                {
                    error -= deltaX;
                    newRasterPoint.y += iy;
                }

                error += deltaY;
                newRasterPoint.x += ix;

                drawPixel(colorBuffer,newRasterPoint.x,newRasterPoint.y, color);
            }
        }
        else
        {
            // error may go below zero
            int error(deltaX - (deltaY >> 1));

            while (newRasterPoint.y != endPoint.y)
            {
                // reduce error while taking into account the corner case of error == 0
                if ((error > 0) || (!error && (iy > 0)))
                {
                    error -= deltaY;
                    newRasterPoint.x += ix;
                }
                // else do nothing

                error += deltaY;
                newRasterPoint.y += iy;

                drawPixel(colorBuffer,newRasterPoint.x,newRasterPoint.y, color);
            }
        }
    };

    switch (algoType)
    {
    case(LineRasterAlgo::DDA):
        drawWithDDAAlgo();
        break;

    case(LineRasterAlgo::BRESENHAM):
        drawWithBresenhamAlgo();
        break;

    default:
        break;
    }

}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat bottom
///////////////////////////////////////////////////////////////////////////////
//
//        (x0,y0)
//          / \
//         /   \
//        /     \
//       /       \
//      /         \
//  (x1,y1)------(x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void drawFlatBottomTriangle(ColorBufferArray& colorBuffer, Triangle& triangle, const size_t color)
{
    auto [point0, point1, point2] = triangle._points;
    const float  MAX_WIDTH = std::abs(point2.x - point1.x);

    float dy1 = point1.y - point0.y;
    float dy2 = point2.y - point0.y;

    if (std::abs(dy1) < EPSILON || std::abs(dy2) < EPSILON)
        return;

    float invSlop1 = (point1.x - point0.x) / dy1;
    float invSlop2 = (point2.x - point0.x) / dy2;

    float xStart = std::round(point0.x);
    float xEnd = std::round(point0.x);

    for (int y = static_cast<int>(point0.y); y <= static_cast<int>(point2.y); y++)  // Ensure proper loop termination
    {
        drawLine(colorBuffer, {static_cast<int>(xStart), y}, {static_cast<int>(xEnd), y}, LineRasterAlgo::BRESENHAM, color);
        xStart += invSlop1;
        xEnd += invSlop2;


        if (std::abs(xStart - xEnd) > MAX_WIDTH)
        {
            xStart = point1.x;
            xEnd   = point2.x;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat top
///////////////////////////////////////////////////////////////////////////////
//
//  (x0,y0)------(x1,y1)
//      \         /
//       \       /
//        \     /
//         \   /
//          \ /
//        (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void drawFlatTopTriangle(ColorBufferArray& colorBuffer, Triangle& triangle, const size_t color)
{
    auto [point0, point1, point2] = triangle._points;
    const float  MAX_WIDTH = std::abs(point1.x - point0.x);

    float dy1 = point2.y - point0.y;
    float dy2 = point2.y - point1.y;

    if (std::abs(dy1) < EPSILON || std::abs(dy2) < EPSILON)
        return;

    float invSlop1 = (point2.x - point0.x) / dy1;
    float invSlop2 = (point2.x - point1.x) / dy2;

    float xStart = std::round(point2.x);
    float xEnd = std::round(point2.x);

    for (int y = static_cast<int>(point2.y); y >= static_cast<int>(point0.y); y--)
    {
        drawLine(colorBuffer, {static_cast<int>(xStart), y}, {static_cast<int>(xEnd), y}, LineRasterAlgo::BRESENHAM, color);
        xStart -= invSlop1;
        xEnd -= invSlop2;

        if (std::abs(xStart - xEnd) > MAX_WIDTH)
        {
            xStart = point0.x;
            xEnd   = point1.x;
        }
    }
}

void drawFilledTriangleFlatBottom(ColorBufferArray& colorBuffer, const Triangle& triangle, const size_t color)
{
    auto triangleSorted{triangle.sortByHeight()};

    if (std::abs(triangleSorted._points[1].y - triangleSorted._points[2].y) < EPSILON)
    {
        drawFlatBottomTriangle(colorBuffer, {triangleSorted}, color);
        return;
    }

    if (std::abs(triangleSorted._points[1].y - triangleSorted._points[0].y) < EPSILON)
    {
        drawFlatTopTriangle(colorBuffer, {triangleSorted}, color);
        return;
    }

    auto midPointOfTriangle {triangleSorted.getMidPoint()};

    Triangle upTriangle{triangleSorted._points[0], triangleSorted._points[1], midPointOfTriangle};
    Triangle bottomTriangle{triangleSorted._points[1], midPointOfTriangle, triangleSorted._points[2]};

    drawFlatBottomTriangle(colorBuffer, upTriangle, color);
    drawFlatTopTriangle(colorBuffer,bottomTriangle,color);

}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat top with a texture
///////////////////////////////////////////////////////////////////////////////
//
//  (x0,y0)------(x1,y1)
//      \         /
//       \       /
//        \     /
//         \   /
//          \ /
//        (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void drawFlatTopTriangleTextured(ColorBufferArray& colorBuffer, const Texture2d& texture,  const TriangleTextured& triangle)
{
}


///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat bottom with texture
///////////////////////////////////////////////////////////////////////////////
//
//        (x0,y0)
//          / \
//         /   \
//        /     \
//       /       \
//      /         \
//  (x1,y1)------(x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void drawFlatBottomTriangleTextured(ColorBufferArray& colorBuffer, const Texture2d& texture,  const TriangleTextured& triangle)
{

}


void drawTexturedTriangle(ColorBufferArray& colorBuffer, const Triangle& triangle, const Texture2d &texture)
{
    const TriangleTextured triangleTextured{triangle};
    const auto sortedTexturedTriangles{triangleTextured.sortByHeight()};

    if (std::abs(sortedTexturedTriangles._pointsWithUV[1].pos.y - sortedTexturedTriangles._pointsWithUV[2].pos.y) < EPSILON)
    {
        drawFlatBottomTriangleTextured(colorBuffer, texture, sortedTexturedTriangles);
        return;
    }

    if (std::abs(sortedTexturedTriangles._pointsWithUV[1].pos.y - sortedTexturedTriangles._pointsWithUV[0].pos.y) < EPSILON)
    {
        drawFlatTopTriangleTextured(colorBuffer, texture, sortedTexturedTriangles);
        return;
    }

    const auto midPointOfTriangle {sortedTexturedTriangles.getMidPoint()};

}


}
