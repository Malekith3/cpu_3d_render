#ifndef PROJECTIONMAT_H
#define PROJECTIONMAT_H
#include "glm/mat4x4.hpp"
namespace Utils
{
    glm::mat4x4 makePerspectiveMat4(float fov, float aspect, float zNear, float zFar);
    glm::vec4 projectWithMat(const glm::mat4x4& projectMatrix, const glm::vec4& vec);
}
#endif //PROJECTIONMAT_H
