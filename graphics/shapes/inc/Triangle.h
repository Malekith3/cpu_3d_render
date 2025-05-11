#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <array>

#include "Vectors.hpp"
#include "glm/fwd.hpp"

struct face_t
{
    int a{};
    int b{};
    int c{};
    uint32_t _color{0xFFFFFFFF};
};

struct triangle_t
{
    std::array<vect2_t<float>,3> _points;
    uint32_t _color{0xFFFFFFFF};

    [[nodiscard]] triangle_t sortByHeight() const;

    [[nodiscard]] vect2_t<float> getMidPoint() const;

    [[nodiscard]] float getAvgDepth() const { return _avgDepth; }

    void setAvgDepth(const float avgDepth) { _avgDepth = avgDepth; }

    triangle_t(const vect2_t<float> point0, const vect2_t<float> point1, const vect2_t<float> point2):_points({{point0,point1,point2}}){}
    explicit triangle_t(const std::array<vect2_t<float>, 3>& points) : _points(points){}
    triangle_t() = default;



private:
    float _avgDepth{0.0f};
};

#endif //TRIANGLE_H
