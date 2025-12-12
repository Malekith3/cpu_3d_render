#ifndef MINIMALSDL2APP_CAMERA_H
#define MINIMALSDL2APP_CAMERA_H

#include "common/inc/Vectors.hpp"

struct Camera
{
    vect3_t<float> _position{0.0f,0.0f,0.0f};
    vect3_t<float> _direction{0.0f, 0.0f,1.0f};
    vect3_t<float> _velocity{0.0f, 0.0f, 0.0f};
    float _yaw = 0.0f;
    float _pitch = 0.0f;

    void updateTick(const float deltaTimeSec, glm::vec3& target)
    {
        _position.y += _velocity.y * deltaTimeSec;

        const auto forwardVelocity = _direction * _velocity.x * deltaTimeSec;
        _position = _position + forwardVelocity;

        // Rotation around Y axis
        auto cameraRotation = glm::mat3(1.0f);
        const auto rotationY = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(_yaw), glm::vec3(0,1,0)));
        const auto rotationX = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(_pitch), glm::vec3(1,0,0)));
        cameraRotation = cameraRotation * rotationY * rotationX;

        // Rotate the forward direction
        _direction = from_glm(cameraRotation * to_glm(_direction));

        // Update target based on new position + direction
        target = to_glm(_position + _direction);
    }
};


#endif //MINIMALSDL2APP_CAMERA_H