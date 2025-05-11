#ifndef LIGHT_H
#define LIGHT_H
#include <Vectors.hpp>

struct  Light
{
    vect3_t<float> _direction;
};

[[nodiscard]] inline const Light& getGlobalLight()
{
    static Light light{._direction = {0,0,1}};
    return light;
}


#endif //LIGHT_H
