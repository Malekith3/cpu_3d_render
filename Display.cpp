#include "Display.h"

#include <algorithm>
#include <cmath>
#include <span>
#include <bits/ranges_algobase.h>

namespace
{
    float fovFactor = 600.0f;
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

void drawRect(ColorBufferArray& colorBuffer, int posX, int posY, size_t width, size_t height, uint32_t color)
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

void drawPixel(ColorBufferArray& colorBuffer, int posX, int posY, uint32_t color)
{
    if (posX == 0 || posY == 0)
    {
        return;
    }

    size_t index{WINDOW_WIDTH*posY + posX};
    if (index < colorBuffer.size())
    {
        colorBuffer[index] = color;
    }
}

void drawRect(ColorBufferArray& colorBuffer,
              const Point& point1, const Point& point2, const Point& point3,
              size_t color, LineRasterAlgo algoType)
{
    drawLine(colorBuffer,point1,point2, algoType, color);
    drawLine(colorBuffer,point2,point3, algoType, color);
    drawLine(colorBuffer,point3,point1, algoType, color);

}

vect2_t<float> project(vect3_t<float>& point)
{
    return fovFactor * vect2_t<float>{point.x / point.z ,point.y / point.z};
}

void drawLine(ColorBufferArray& colorBuffer, const Point& startPoint, const Point& endPoint, LineRasterAlgo algoType, uint32_t color)
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
                // reduce error, while taking into account the corner case of error == 0
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
                // reduce error, while taking into account the corner case of error == 0
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


void drawFlatUpTriangle(ColorBufferArray& colorBuffer, triangle_t& triangle, size_t color)
{
    auto [point0, point1, point2] = triangle._points;
    float invSlop1 = (point1.x - point0.x) / (point1.y - point0.y);
    float invSlop2 = (point2.x - point0.x) / (point2.y - point0.y);

    float xStart = point0.x;
    float xEnd = point0.x;

    for (float y = point0.y; y <= point2.y; y++)
    {
        drawLine(colorBuffer,{xStart,y}, {xEnd,y},LineRasterAlgo::DDA,color);
        xStart+= invSlop1;
        xEnd+=invSlop2;
    }

}

void drawFilledTriangleFlatBottom(ColorBufferArray& colorBuffer, const triangle_t& triangle, const size_t color)
{
    auto triangleSorted{triangle.sortByHeight()};
    auto midPointOfTriangle {triangleSorted.getMidPoint()};
    triangle_t upTriangle{triangleSorted._points[0], triangleSorted._points[1], midPointOfTriangle};
    drawFlatUpTriangle(colorBuffer, upTriangle, color);

}


}
