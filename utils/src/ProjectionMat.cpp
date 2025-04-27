#include "ProjectionMat.h"
namespace Utils
{
glm::mat4x4 makePerspectiveMat4(const float fov, const float aspect, const float zNear, const float zFar)
{
    /**
    * @brief Creates a perspective projection matrix for 3D rendering.
    *
    * The matrix transforms 3D world coordinates into 2D screen coordinates, simulating perspective.
    * The elements of the matrix are based on the field of view (FOV), aspect ratio, and near/far plane distances.
    *
    * Matrix Layout:
    * | (h/w)*1/tan(fov/2)             0               0                0  |
    * |                0      1/tan(fov/2)             0                0  |
    * |                0               0      zf/(zf-zn)  (-zf*zn)/(zf-zn) |
    * |                0               0               1                0  |
    *
    * Where:
    * - h/w is the aspect ratio.
    * - 1/tan(fov/2) determines the scaling of the X and Y axes based on the FOV.
    * - zf/(zf-zn) scales the Z axis to map depth between the near and far planes.
    * - (-zf*zn)/(zf-zn) is used to properly translate the Z values .
    *
    **/

    const float invTangFov = 1.0f / std::tan(fov / 2.0f);
    const float zFarNearRatio = zFar / (zFar - zNear);

    glm::mat4x4 perspectiveMat{0};
    perspectiveMat[0][0] = invTangFov / aspect;
    perspectiveMat[1][1] = invTangFov;
    perspectiveMat[2][2] = zFarNearRatio;
    perspectiveMat[2][3] = 1.0f;
    perspectiveMat[3][2] = (-zNear * zFar)/(zFar - zNear);


    return perspectiveMat;
}

glm::vec4 projectWithMat(const glm::mat4x4& projectMatrix, const glm::vec4& vec)
{
    glm::vec4 projectedVect =   projectMatrix * vec ;
    if (projectedVect.w != 0)
    {
        projectedVect /= projectedVect.w;
    }

    return projectedVect;
}
}