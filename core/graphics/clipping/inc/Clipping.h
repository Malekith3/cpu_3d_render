#ifndef MINIMALSDL2APP_CLIPPING_H
#define MINIMALSDL2APP_CLIPPING_H

#include <common/inc/Vectors.hpp>

enum
{
    LEFT_FRUSTUM_PLANE,
    RIGHT_FRUSTUM_PLANE,
    TOP_FRUSTUM_PLANE,
    BOTTOM_FRUSTUM_PLANE,
    NEAR_FRUSTUM_PLANE,
    FAR_FRUSTUM_PLANE
};

struct Plane
{
    vect2_t<float> point{};
    vect3_t<float> norm{};
};

#endif //MINIMALSDL2APP_CLIPPING_H