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
Frustum::Frustum(const float fov, const float zNear, const float zFar)
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

    const auto halfFOV = fov / 2.0f;
    const auto sinHalfFOV = std::sin(halfFOV);
    const auto cosHalfFOV = std::cos(halfFOV);

    leftPlane.norm = {cosHalfFOV, 0, sinHalfFOV};
    rightPlane.norm = {-cosHalfFOV, 0, sinHalfFOV};
    topPlane.norm = {0, -cosHalfFOV, sinHalfFOV};
    bottomPlane.norm = {0, cosHalfFOV, sinHalfFOV};
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

void Frustum::clipAgainstPlanes(const Plane &plane, Polygon &polygon) {
    if (polygon.numVertices == 0)
    {
        DEBUG_LOG("Got Polygon without vertices");
        return;
    }

    std::array<vect3_t<float>, MAX_NUM_POLY_VERTICES> insideVertices{};
    std::size_t insideCount = 0u;

    auto signedDistance = [&plane](const vect3_t<float>& p) {
        return (p - plane.point).dot(plane.norm);
    };

    constexpr float EPSILON = std::numeric_limits<float>::epsilon() * 10;
    auto isInside = [&](const float d) {
        return d >= -EPSILON;
    };

    const std::size_t originalCount = polygon.numVertices;

    // Classic Sutherland–Hodgman over the polygon edges
    for (std::size_t i = 0; i < originalCount; ++i)
    {
        const auto& current = polygon.vertices[i];
        const auto& previous = polygon.vertices[(i + originalCount - 1) % originalCount];

        const float currentDist  = signedDistance(current);
        const float previousDist = signedDistance(previous);

        const bool currentInside  = isInside(currentDist);
        const bool previousInside = isInside(previousDist);

        // Case 1: edge crosses the plane (inside <-> outside)
        if (currentInside != previousInside)
        {
            const float denom = (previousDist - currentDist);

            // Just in case: avoid division by ~0 (shouldn't really happen if we’re here)
            if (std::abs(denom) <= EPSILON)
            {
                DEBUG_LOG("Division by zero");
                continue;
            }

            const float t = previousDist / denom;

            const vect3_t<float> intersection = previous + (current - previous) * t; //Qp + t(Qc - Qp)

            if (insideCount < MAX_NUM_POLY_VERTICES) {
                insideVertices[insideCount++] = intersection;
            }

        }

        // Case 2: current vertex is inside -> keep it
        if (currentInside && insideCount < MAX_NUM_POLY_VERTICES)
        {
                insideVertices[insideCount++] = current;
        }
    }



    if (insideCount == 0)
        return;

    polygon.numVertices = insideCount;
    std::ranges::copy_n(insideVertices.begin(), insideCount, polygon.vertices.begin());

    DEBUG_LOG("Clipped polygon with {} vertices", insideCount);

}

Polygon::Polygon(std::array<vect3_t<float>, TRIANGLE_VERTICES_COUNT> &triangleVert)
{
    std::ranges::copy_n(triangleVert.begin(), TRIANGLE_VERTICES_COUNT, vertices.begin());
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


