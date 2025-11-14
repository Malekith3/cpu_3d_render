#ifndef PROJECTIONMAT_H
#define PROJECTIONMAT_H
#include "glm/mat4x4.hpp"
namespace Utils
{
    glm::mat4x4 makePerspectiveMat4(float fov, float aspect, float zNear, float zFar);
    glm::vec4 projectWithMat(const glm::mat4x4& projectMatrix, const glm::vec4& vec);
    glm::mat4x4 lookAtMat(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);
}
#endif //PROJECTIONMAT_H
