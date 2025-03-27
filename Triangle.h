#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <array>

#include "Vectors.h"

struct face_t
{
    int a;
    int b;
    int c;
};

struct triangle_t
{
    std::array<vect2_t<float>,3> _points;


    [[nodiscard]] triangle_t sortByHeight() const;

    [[nodiscard]] vect2_t<float> getMidPoint() const;
};

#endif //TRIANGLE_H
