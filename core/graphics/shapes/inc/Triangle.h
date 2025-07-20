#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "graphics/textures/inc/Textures.h"

#include "common/inc/Vectors.hpp"
#include "glm/vec4.hpp"

#include <array>

#include "utils/inc/GlmAdapter.h"
#include "utils/inc/GlmAdapter.h"

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
    std::array<glm::vec4, 3> _points;
    uint32_t _color{0xFFFFFFFF};
    std::array<Texture2d,3> textCoord{};

    [[nodiscard]] Triangle sortByHeight() const;

    [[nodiscard]] vect2_t<float> getMidPoint() const;

    [[nodiscard]] float getAvgDepth() const { return _avgDepth; }

    void setAvgDepth(const float avgDepth) { _avgDepth = avgDepth; }

    Triangle(const glm::vec4 point0, const glm::vec4 point1, const glm::vec4 point2):_points({{point0,point1,point2}}){}
    explicit Triangle(const std::array<glm::vec4, 3>& points) : _points(points){}
    Triangle() = default;



private:
    float _avgDepth{0.0f};
};

struct Vertex2D
{
    glm::vec4 pos{};
    Texture2d uv{};
};

struct TriangleTextured
{
    std::array<Vertex2D,3> _pointsWithUV{};

    TriangleTextured() = default;

    explicit TriangleTextured(const Triangle &triangle);
    explicit TriangleTextured(const std::array<Vertex2D,3>& pointsWithUV);

    [[nodiscard]] TriangleTextured sortByHeight() const;

    [[nodiscard]] Vertex2D getMidPoint() const;
    [[nodiscard]] std::array<glm::vec4, 3> getPoints() const;
    [[nodiscard]] std::array<Texture2d,3> getUVs() const;
};

#endif //TRIANGLE_H
