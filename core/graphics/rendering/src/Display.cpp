#include "graphics/rendering/inc/Display.h"
#include "graphics/shapes/inc/Triangle.h"

#include "common/inc/Colors.h"
#include "common/inc/CommonDefines.h"
#include "common/inc/Vectors.hpp"
#include "common/inc/Math3D.h"

#include <algorithm>
#include <cmath>
#include <span>

#include "glm/mat4x4.hpp"

#define internal static

namespace
{
    float fovFactor = 600.0f;
    constexpr float EPSILON = 1e-4f; // Machine epsilon for float precision
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

    Triangle upTriangle{triangleSorted._points[0], triangleSorted._points[1], {midPointOfTriangle.x, midPointOfTriangle.y,0,0}};
    Triangle bottomTriangle{triangleSorted._points[1], {midPointOfTriangle.x, midPointOfTriangle.y,0,0}, triangleSorted._points[2]};

    drawFlatBottomTriangle(colorBuffer, upTriangle, color);
    drawFlatTopTriangle(colorBuffer,bottomTriangle,color);

}

internal void drawTexel(ColorBufferArray& colorBuffer,
                        std::vector<uint32_t>& texture,
                        const TriangleTextured& triangle,
                        int xCoord,
                        int yCoord)
{
    vect2_t<float> pointP{static_cast<float>(xCoord), static_cast<float>(yCoord)};
    const auto& [pointA, pointB, pointC] = triangle.getPoints();
    const auto& [pointAUV, pointBUV, pointCUV] = triangle.getUVs();
    const auto& barycentricWeightsResult =  Math3D::BarycentricWeights({pointA.x, pointA.y}, {pointB.x, pointB.y}, {pointC.x, pointC.y},pointP);
    const auto& [alpha, beta, gama] = barycentricWeightsResult;

    float interpolatedU = (pointAUV.u / pointA.w) * alpha + (pointBUV.u / pointB.w) * beta + (pointCUV.u / pointC.w) * gama;
    float interpolatedV = (pointAUV.v / pointA.w) * alpha + (pointBUV.v / pointB.w) * beta + (pointCUV.v / pointC.w) * gama;
    
    const float interpolatedReciprocalW = alpha * (1/ pointA.w) + beta * (1/ pointB.w) + gama * (1/ pointC.w);

    interpolatedU /= interpolatedReciprocalW;
    interpolatedV /= interpolatedReciprocalW;

    const int texelIndexX = static_cast<int>(std::clamp(abs(interpolatedU), 0.0f, 1.0f) * TEXTURE_WIDTH );
    const int texelIndexY = static_cast<int>(std::clamp(abs(interpolatedV), 0.0f, 1.0f) * TEXTURE_HEIGHT);

    if (const size_t textureIndex = (TEXTURE_WIDTH * texelIndexY) + texelIndexX; textureIndex < texture.size())
    {
        drawPixel(colorBuffer, xCoord, yCoord, texture[textureIndex]);
    }
    else
    {
        drawPixel(colorBuffer, xCoord, yCoord, ERROR_COLOR);
    }

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
void drawFlatTopTriangleTextured(ColorBufferArray& colorBuffer, std::vector<uint32_t> &texture, TriangleTextured &triangle)
{
    auto& vertices = triangle._pointsWithUV;

    if (vertices[0].pos.x > vertices[1].pos.x)
    {
        std::swap(vertices[0], vertices[1]);
    }

    auto& v0 = vertices[0]; // top-left
    auto& v1 = vertices[1]; // top-right
    auto& v2 = vertices[2]; // bottom

    const float area = (v1.pos.x - v0.pos.x) * (v2.pos.y - v0.pos.y) -
                       (v2.pos.x - v0.pos.x) * (v1.pos.y - v0.pos.y);

    if (std::abs(area) < EPSILON)
        return; // Degenerate triangle

    // Calculate slopes for left and right edges
    const float dyLeft = v2.pos.y - v0.pos.y;
    const float dyRight = v2.pos.y - v1.pos.y;

    if (std::abs(dyLeft) <= EPSILON || std::abs(dyRight) <= EPSILON)
        return;

    const float invSlopeLeft = (v2.pos.x - v0.pos.x) / dyLeft;
    const float invSlopeRight = (v2.pos.x - v1.pos.x) / dyRight;

    const int startY = static_cast<int>(std::ceil(v0.pos.y));
    const int endY = static_cast<int>(std::ceil(v2.pos.y));

    for (int y = startY; y < endY; y++)
    {
        float xLeft = v0.pos.x + (y - v0.pos.y) * invSlopeLeft;
        float xRight = v1.pos.x + (y - v1.pos.y) * invSlopeRight;

        const int xStart = static_cast<int>(std::ceil(std::min(xLeft, xRight)));
        const int xEnd = static_cast<int>(std::ceil(std::max(xLeft, xRight)));

        for (int x = xStart; x < xEnd; x++)
        {
            drawTexel(colorBuffer, texture, triangle, x, y);
        }
    }
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
void drawFlatBottomTriangleTextured(ColorBufferArray& colorBuffer, std::vector<uint32_t> &texture, TriangleTextured &triangle)
{
    auto& vertices = triangle._pointsWithUV;

    // Ensure vertices[1] is left and vertices[2] is right on the flat bottom
    if (vertices[1].pos.x > vertices[2].pos.x)
    {
        std::swap(vertices[1], vertices[2]);
    }

    auto& v0 = vertices[0]; // top
    auto& v1 = vertices[1]; // bottom-left
    auto& v2 = vertices[2]; // bottom-right

    const float area = (v1.pos.x - v0.pos.x) * (v2.pos.y - v0.pos.y) -
                       (v2.pos.x - v0.pos.x) * (v1.pos.y - v0.pos.y);

    if (std::abs(area) < EPSILON)
        return; // Degenerate triangle

    const float dyLeft = v1.pos.y - v0.pos.y;
    const float dyRight = v2.pos.y - v0.pos.y;

    if (std::abs(dyLeft) <= EPSILON || std::abs(dyRight) <= EPSILON)
        return;

    const float invSlopeLeft = (v1.pos.x - v0.pos.x) / dyLeft;
    const float invSlopeRight = (v2.pos.x - v0.pos.x) / dyRight;

    const int startY = static_cast<int>(std::ceil(v0.pos.y));
    const int endY = static_cast<int>(std::ceil(v1.pos.y));

    for (int y = startY; y < endY; y++)
    {
        float xLeft = v0.pos.x + (y - v0.pos.y) * invSlopeLeft;
        float xRight = v0.pos.x + (y - v0.pos.y) * invSlopeRight;

        int xStart = static_cast<int>(std::ceil(std::min(xLeft, xRight)));
        int xEnd = static_cast<int>(std::ceil(std::max(xLeft, xRight)));

        for (int x = xStart; x < xEnd; x++)
        {
            drawTexel(colorBuffer, texture, triangle, x, y);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Main triangle drawing function with proper triangle splitting
///////////////////////////////////////////////////////////////////////////////
void drawTexturedTriangle(ColorBufferArray& colorBuffer, const Triangle& triangle, std::vector<uint32_t> &texture)
{
    const TriangleTextured triangleTextured{triangle};
    auto vertices = triangleTextured._pointsWithUV; // Make a copy for sorting

    // Sort vertices by y-coordinate (ascending)
    std::ranges::sort(vertices, [](const Vertex2D& a, const Vertex2D& b) {
        return a.pos.y < b.pos.y;
    });

    const auto& v0 = vertices[0]; // top vertex
    const auto& v1 = vertices[1]; // middle vertex
    const auto& v2 = vertices[2]; // bottom vertex

    // Check for degenerate cases
    if (std::abs(v0.pos.y - v2.pos.y) < EPSILON)
        return;

    if (std::abs(v1.pos.y - v2.pos.y) < EPSILON)
    {
        TriangleTextured flatBottomTri;
        flatBottomTri._pointsWithUV = {v0, v1, v2};
        drawFlatBottomTriangleTextured(colorBuffer, texture, flatBottomTri);
        return;
    }

    if (std::abs(v0.pos.y - v1.pos.y) < EPSILON)
    {
        TriangleTextured flatTopTri;
        flatTopTri._pointsWithUV = {v0, v1, v2};
        drawFlatTopTriangleTextured(colorBuffer, texture, flatTopTri);
        return;
    }

    // Case 3: General triangle - split into flat-bottom and flat-top parts

    // Find the x-coordinate where the horizontal line at v1.y intersects the long edge (v0->v2)
    const float slopeMiddlePoint = (v1.pos.y - v0.pos.y) / (v2.pos.y - v0.pos.y);
    const float splitX = v0.pos.x + slopeMiddlePoint * (v2.pos.x - v0.pos.x);

    // Perspective-correct interpolation for UV coordinates and other attributes
    const float u0OverW0 = v0.uv.u / v0.pos.w;
    const float u2OverW2 = v2.uv.u / v2.pos.w;
    const float v0OverW0 = v0.uv.v / v0.pos.w;
    const float v2OverW2 = v2.uv.v / v2.pos.w;
    const float reciprocalW0 = 1.0f / v0.pos.w;
    const float reciprocalW2 = 1.0f / v2.pos.w;

    const float interpolatedUOverW = u0OverW0 + slopeMiddlePoint * (u2OverW2 - u0OverW0);
    const float interpolatedVOverW = v0OverW0 + slopeMiddlePoint * (v2OverW2 - v0OverW0);
    const float interpolatedOneOverW = reciprocalW0 + slopeMiddlePoint * (reciprocalW2 - reciprocalW0);
    const float interpolatedW = 1.0f / interpolatedOneOverW;

    const float splitU = interpolatedUOverW * interpolatedW;
    const float splitV = interpolatedVOverW * interpolatedW;

    // Create the split vertex
    Vertex2D splitVertex;
    splitVertex.pos.x = splitX;
    splitVertex.pos.y = v1.pos.y;
    splitVertex.pos.z = v0.pos.z + slopeMiddlePoint * (v2.pos.z - v0.pos.z); // Linear interpolation for z
    splitVertex.pos.w = interpolatedW;
    splitVertex.uv.u = splitU;
    splitVertex.uv.v = splitV;

    // Draw upper triangle (flat-bottom)
    // Top vertex: v0, Bottom edge: v1 and splitVertex
    TriangleTextured upperTri;
    upperTri._pointsWithUV = {v0, v1, splitVertex};
    drawFlatBottomTriangleTextured(colorBuffer, texture, upperTri);

    // Draw lower triangle (flat-top)
    // Top edge: v1 and splitVertex, Bottom vertex: v2
    TriangleTextured lowerTri;
    lowerTri._pointsWithUV = {v1, splitVertex, v2};
    drawFlatTopTriangleTextured(colorBuffer, texture, lowerTri);
}
}
