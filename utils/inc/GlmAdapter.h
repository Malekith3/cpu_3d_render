#ifndef GLM_ADAPTER_H
#define GLM_ADAPTER_H

#include "Vectors.hpp"
#include <glm/glm.hpp>

// --- vect2_t <-> glm::vec2 ---
template <typename T>
glm::vec<2, T, glm::defaultp> to_glm(const vect2_t<T>& v)
{
    return glm::vec<2, T, glm::defaultp>(v.x, v.y);
}

template <typename T>
vect2_t<T> from_glm(const glm::vec<2, T, glm::defaultp>& v)
{
    return vect2_t<T>{v.x, v.y};
}

// --- vect3_t <-> glm::vec3 ---
template <typename T>
glm::vec<3, T, glm::defaultp> to_glm(const vect3_t<T>& v)
{
    return glm::vec<3, T, glm::defaultp>(v.x, v.y, v.z);
}

template <typename T>
vect3_t<T> from_glm(const glm::vec<3, T, glm::defaultp>& v)
{
    return vect3_t<T>{v.x, v.y, v.z};
}

template <typename T>
vect3_t<T> fromGLMVect3(const glm::vec<4, T, glm::defaultp>& v)
{
    return vect3_t<T>{v.x, v.y, v.z};
}
#endif // GLM_ADAPTER_H
