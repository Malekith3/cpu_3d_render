#ifndef MINIMALSDL2APP_CLIPPING_H
#define MINIMALSDL2APP_CLIPPING_H

#include <array>
#include <functional>
#include <vector>

#include <common/inc/Vectors.hpp>
#include "graphics/textures/inc/Textures.h"

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
    std::array<Texture2d, MAX_NUM_POLY_VERTICES> texCoords{};
    size_t numVertices{};
    Polygon() = default;
    explicit Polygon(std::array<vect3_t<float>, TRIANGLE_VERTICES_COUNT>& triangleVert , std::array<Texture2d, TRIANGLE_VERTICES_COUNT> triTexCoords);

    [[nodiscard]] std::vector<std::array<vect3_t<float>, TRIANGLE_VERTICES_COUNT>>  polygon2Triangles() const;
    [[nodiscard]] std::vector<std::array<Texture2d, TRIANGLE_VERTICES_COUNT>>  polygon2TrianglesTex() const;
};

struct Frustum
{
    using ClipFn = std::function<Polygon(const Polygon&, Plane&)>;

    explicit Frustum(const float fovX, const float fovY, const float zNear, const float zFar);
    [[nodiscard]] const std::array<Plane, PlanesNames::NUMBER_OF_PLANES>& getPlanes() const {return planes;}

    //Clip given polygon aginst current initilised frustum that was configured with ctor fov, zNear, zFar
    [[nodiscard]] Polygon ClipPolygon(const Polygon& polygon) const;
private:
    static void clipAgainstPlanes(const Plane& plane, Polygon& polygon);
    std::array<Plane, PlanesNames::NUMBER_OF_PLANES> planes{};
};

#endif //MINIMALSDL2APP_CLIPPING_H