#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "graphics/textures/inc/Textures.h"

#include "common/inc/Vectors.hpp"

#include <array>

struct Face
{
    int a{};
    int b{};
    int c{};
    uint32_t color{0xFFFFFFFF};
    Texture2d a_uv{};
    Texture2d b_uv{};
    Texture2d c_uv{};
};

struct Triangle
{
    std::array<vect2_t<float>,3> _points;
    uint32_t _color{0xFFFFFFFF};
    std::array<Texture2d,3> textCoord{};

    [[nodiscard]] Triangle sortByHeight() const;

    [[nodiscard]] vect2_t<float> getMidPoint() const;

    [[nodiscard]] float getAvgDepth() const { return _avgDepth; }

    void setAvgDepth(const float avgDepth) { _avgDepth = avgDepth; }

    Triangle(const vect2_t<float> point0, const vect2_t<float> point1, const vect2_t<float> point2):_points({{point0,point1,point2}}){}
    explicit Triangle(const std::array<vect2_t<float>, 3>& points) : _points(points){}
    Triangle() = default;



private:
    float _avgDepth{0.0f};
};

struct Vertex2D
{
    vect2_t<float> pos{};
    Texture2d uv{};
};

struct TriangleTextured
{
    std::array<Vertex2D,3> _pointsWithUV{};

    TriangleTextured() = default;

    explicit TriangleTextured(const Triangle &triangle);

    [[nodiscard]] TriangleTextured sortByHeight() const;

    [[nodiscard]] Vertex2D getMidPoint() const;
};

#endif //TRIANGLE_H
