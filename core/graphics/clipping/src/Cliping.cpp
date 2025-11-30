#include <graphics/clipping/inc/Clipping.h>

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
Frustum::Frustum(const float fov, const float zNear, const float zFar) {
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

    for (const auto& plane : planes) {
        clipAgainstPlanes(plane, outputPolygon);
    }

    return outputPolygon;
}

void Frustum::clipAgainstPlanes(const Plane &plane, Polygon &polygon) const
{

}

Polygon::Polygon(std::array<vect3_t<float>, TRIANGLE_VERTICES_COUNT> &triangleVert)
{
    std::ranges::copy_n(triangleVert.begin(), TRIANGLE_VERTICES_COUNT, vertices.begin());
}

