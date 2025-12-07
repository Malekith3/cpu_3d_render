#ifndef MINIMALSDL2APP_CLIPPING_H
#define MINIMALSDL2APP_CLIPPING_H

#include <algorithm>
#include <array>
#include <functional>
#include <vector>
#include <ranges>
#include <common/inc/Vectors.hpp>

enum PlanesNames : size_t
{
    LEFT_FRUSTUM_PLANE,
    RIGHT_FRUSTUM_PLANE,
    TOP_FRUSTUM_PLANE,
    BOTTOM_FRUSTUM_PLANE,
    NEAR_FRUSTUM_PLANE,
    FAR_FRUSTUM_PLANE,
    NUMBER_OF_PLANES
};

constexpr size_t MAX_NUM_POLY_VERTICES = 10;
static constexpr size_t TRIANGLE_VERTICES_COUNT = 3;

struct Plane
{
    vect3_t<float> point{};
    vect3_t<float> norm{};
};

struct Polygon
{
    std::array<vect3_t<float>, MAX_NUM_POLY_VERTICES> vertices{};
    size_t numVertices{};
    Polygon() = default;
    explicit Polygon(std::array<vect3_t<float>, TRIANGLE_VERTICES_COUNT>& triangleVert);

    [[nodiscard]] std::vector<std::array<vect3_t<float>, TRIANGLE_VERTICES_COUNT>>  polygon2Triangles() const;
};

struct Frustum
{
    using ClipFn = std::function<Polygon(const Polygon&, Plane&)>;

    explicit Frustum(float fov, float zNear, float zFar);
    [[nodiscard]] const std::array<Plane, PlanesNames::NUMBER_OF_PLANES>& getPlanes() const {return planes;}

    //Clip given polygon aginst current initilised frustum that was configured with ctor fov, zNear, zFar
    [[nodiscard]] Polygon ClipPolygon(const Polygon& polygon) const;
private:
    static void clipAgainstPlanes(const Plane& plane, Polygon& polygon);
    std::array<Plane, PlanesNames::NUMBER_OF_PLANES> planes{};
};

#endif //MINIMALSDL2APP_CLIPPING_H