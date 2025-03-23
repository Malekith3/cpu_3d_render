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
    std::array<vect2_t<float>,3> points;
};

#endif //TRIANGLE_H
