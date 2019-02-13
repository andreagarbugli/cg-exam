#include <iostream>
#include "Camera.hpp"

namespace Engine
{
    void Camera::Update(double deltaTime)
    {
        auto velocity = static_cast<float>(_movementSpeed * deltaTime);

        if (_inputManager->IsKeyPressed(KeyboardKey::KEY_W))
        {
            _position += _front * velocity;
        }

        if (_inputManager->IsKeyPressed(KeyboardKey::KEY_S))
        {
            _position -= _front * velocity;
        }

        if (_inputManager->IsKeyPressed(KeyboardKey::KEY_A))
        {
            _position -= _right * velocity;
        }

        if (_inputManager->IsKeyPressed(KeyboardKey::KEY_D))
        {
            _position += _right * velocity;
        }

        auto xChange = _inputManager->GetXChange();
        auto yChange = _inputManager->GetYChange();

        auto turnVelocity = _turnSpeed;

        xChange *= turnVelocity;
        yChange *= turnVelocity;

        _yaw += xChange;
        _pitch += yChange;

        if (_pitch > 89.0f)
        {
            _pitch = 89.0f;
        }

        if (_pitch < -89.0f)
        {
            _pitch = -89.0f;
        }

        _front.x = std::cos(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));
        _front.y = std::sin(glm::radians(_pitch));
        _front.z = std::sin(glm::radians(_yaw)) * std::cos(glm::radians(_pitch));

        _front = glm::normalize(_front);
        _right = glm::normalize(glm::cross(_front, _worldUp));
        _up = glm::normalize(glm::cross(_right, _front));

        _viewMatrix = _CalculateViewMatrix();
    }

    glm::mat4 Camera::_CalculateViewMatrix()
    {
        return glm::lookAt(_position, _position + _front, _up);
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return _viewMatrix;
    }

    glm::vec3 Camera::GetDirecton() const
    {
        return glm::normalize(_front);
    }

    void Camera::SetMovimentSpeed(float speed)
    {
        _movementSpeed = speed;
    }
}