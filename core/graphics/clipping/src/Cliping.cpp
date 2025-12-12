#include <graphics/clipping/inc/Clipping.h>

#include "logger/LogHelper.h"

///////////////////////////////////////////////////////////////////////////////
// Frustum planes are defined by a point and a normal vector
///////////////////////////////////////////////////////////////////////////////
// Near plane   :  P=(0, 0, znear), N=(0, 0,  1)
// Far plane    :  P=(0, 0, zfar),  N=(0, 0, -1)
// Top plane    :  P=(0, 0, 0),     N=(0, -cos(fov/2), sin(fov/2))
// Bottom plane :  P=(0, 0, 0),     N=(0, cos(fov/2), sin(fov/2))
// Left plane   :  P=(0, 0, 0),     N=(cos(fov/2), 0, sin(fov/2))
// Right plane  :  P=(0, 0, 0),     N=(-cos(fov/2), 0, sin(fov/2))
///////////////////////////////////////////////////////////////////////////////
//
//           /|\
//         /  | |
//       /\   | |
//     /      | |
//  P*|-->  <-|*|   ----> +z-axis
//     \      | |
//       \/   | |
//         \  | |
//           \|/
//
///////////////////////////////////////////////////////////////////////////////
Frustum::Frustum(const float fovX, const float fovY, const float zNear, const float zFar)
{
    auto &leftPlane = planes[PlanesNames::LEFT_FRUSTUM_PLANE];
    auto &rightPlane = planes[PlanesNames::RIGHT_FRUSTUM_PLANE];
    auto &bottomPlane = planes[PlanesNames::BOTTOM_FRUSTUM_PLANE];
    auto &topPlane = planes[PlanesNames::TOP_FRUSTUM_PLANE];
    auto &nearPlane = planes[PlanesNames::NEAR_FRUSTUM_PLANE];
    auto &farPlane = planes[PlanesNames::FAR_FRUSTUM_PLANE];

    leftPlane.point = {0, 0, 0};
    rightPlane.point = {0, 0, 0};
    bottomPlane.point = {0, 0, 0};
    topPlane.point = {0, 0, 0};
    nearPlane.point = {0, 0, zNear};
    farPlane.point = {0, 0, zFar};

    const auto halfFOVX = fovX / 2.0f;
    const auto halfFOVY = fovY / 2.0f;

    const auto sinHalfFOVX = std::sin(halfFOVX);
    const auto cosHalfFOVX = std::cos(halfFOVX);

    const auto sinHalfFOVY = std::sin(halfFOVY);
    const auto cosHalfFOVY = std::cos(halfFOVY);

    leftPlane.norm = {cosHalfFOVX, 0, sinHalfFOVX};
    rightPlane.norm = {-cosHalfFOVX, 0, sinHalfFOVX};
    topPlane.norm = {0, -cosHalfFOVY, sinHalfFOVY};
    bottomPlane.norm = {0, cosHalfFOVY, sinHalfFOVY};
    nearPlane.norm = {0, 0,1};
    farPlane.norm = {0, 0,-1};
}

Polygon Frustum::ClipPolygon(const Polygon &polygon) const
{
    Polygon outputPolygon{polygon};

    for (const auto& plane : planes)
    {
        clipAgainstPlanes(plane, outputPolygon);
    }

    return outputPolygon;
}

void Frustum::clipAgainstPlanes(const Plane &plane, Polygon &polygon)
{
    if (polygon.numVertices == 0)
    {
        return;
    }

    std::array<vect3_t<float>, MAX_NUM_POLY_VERTICES> insideVertices{};
    std::array<Texture2d, MAX_NUM_POLY_VERTICES> insideTexCoords{};
    std::size_t insideCount = 0u;

    auto signedDistance = [&plane](const vect3_t<float>& p) {
        return (p - plane.point).dot(plane.norm);
    };

    constexpr float EPSILON = std::numeric_limits<float>::epsilon() * 10;
    auto isInside = [&](const float d) {
        return d >= -EPSILON;
    };

    auto lerpUV = [](const Texture2d& a, const Texture2d& b, float t) -> Texture2d {
        return Texture2d{
            a.u + (b.u - a.u) * t,
            a.v + (b.v - a.v) * t
        };
    };

    const std::size_t originalCount = polygon.numVertices;

    // Classic Sutherland–Hodgman over the polygon edges
    for (std::size_t i = 0; i < originalCount; ++i)
    {
        const std::size_t prevIndex = (i + originalCount - 1) % originalCount;

        const auto& currentPos  = polygon.vertices[i];
        const auto& previousPos = polygon.vertices[prevIndex];

        const auto& currentUV  = polygon.texCoords[i];
        const auto& previousUV = polygon.texCoords[prevIndex];

        const float currentDist  = signedDistance(currentPos);
        const float previousDist = signedDistance(previousPos);

        const bool currentInside  = isInside(currentDist);
        const bool previousInside = isInside(previousDist);

        // Case 1: edge crosses the plane (inside <-> outside)
        if (currentInside != previousInside)
        {
            const float denom = (previousDist - currentDist);
            if (std::abs(denom) <= EPSILON)
            {
                continue;
            }

            const float t = previousDist / denom;

            const vect3_t<float> intersectionPos = previousPos + (currentPos - previousPos) * t;
            const Texture2d intersectionUV = lerpUV(previousUV, currentUV, t);

            if (insideCount < MAX_NUM_POLY_VERTICES)
            {
                insideVertices[insideCount] = intersectionPos;
                insideTexCoords[insideCount] = intersectionUV;
                ++insideCount;
            }
        }

        // Case 2: current vertex is inside -> keep it
        if (currentInside && insideCount < MAX_NUM_POLY_VERTICES)
        {
            insideVertices[insideCount] = currentPos;
            insideTexCoords[insideCount] = currentUV;
            ++insideCount;
        }
    }

    if (insideCount == 0)
    {
        polygon.numVertices = 0;
        return;
    }

    polygon.numVertices = insideCount;
    std::ranges::copy_n(insideVertices.begin(), insideCount, polygon.vertices.begin());
    std::ranges::copy_n(insideTexCoords.begin(), insideCount, polygon.texCoords.begin());
}

Polygon::Polygon(std::array<vect3_t<float>, TRIANGLE_VERTICES_COUNT> &triangleVert, std::array<Texture2d, TRIANGLE_VERTICES_COUNT> triTexCoords)
{
    std::ranges::copy_n(triangleVert.begin(), TRIANGLE_VERTICES_COUNT, vertices.begin());
    std::ranges::copy_n(triTexCoords.begin(), TRIANGLE_VERTICES_COUNT, texCoords.begin());
    numVertices = TRIANGLE_VERTICES_COUNT;
}

std::vector<std::array<vect3_t<float>, 3>> Polygon::polygon2Triangles() const
{
    std::vector<std::array<vect3_t<float>, 3>> triangles;

    if (numVertices < 3)
        return triangles;

    triangles.reserve(numVertices - 2);

    for (size_t i = 1; i + 1 < numVertices; i++)
    {
        triangles.push_back({ vertices[0], vertices[i], vertices[i + 1] });
    }

    return triangles;
}

std::vector<std::array<Texture2d, TRIANGLE_VERTICES_COUNT>> Polygon::polygon2TrianglesTex() const
{
    std::vector<std::array<Texture2d, TRIANGLE_VERTICES_COUNT>> triangles;

    if (numVertices < 3)
        return triangles;

    triangles.reserve(numVertices - 2);

    for (size_t i = 1; i + 1 < numVertices; i++)
    {
        triangles.push_back({ texCoords[0], texCoords[i], texCoords[i + 1] });
    }

    return triangles;
}

