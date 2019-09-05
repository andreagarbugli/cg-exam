#include <iostream>
#include "Camera.hpp"

namespace Engine
{
    /**************************/
    /*      BASIC CAMERA      */
    /**************************/

    glm::mat4 BasicCamera::_CalculateViewMatrix()
    {
        return glm::lookAt(_position, _position + _front, _up);
    }

    glm::mat4 BasicCamera::GetViewMatrix() const
    {
        return _viewMatrix;
    }

    glm::vec3 BasicCamera::GetDirecton() const
    {
        return glm::normalize(_front);
    }

    glm::vec3 BasicCamera::GetFront()
    {
        return _front;
    }


    void BasicCamera::Update(double deltaTime)
    {
        _UpdatePosition(deltaTime);

        _viewMatrix = _CalculateViewMatrix();
    }

    void BasicCamera::_UpdatePosition(double deltaTime)
    {

    }

    /**************************/
    /*         CAMERA         */
    /**************************/

    void Camera::_UpdatePosition(double deltaTime)
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
    }

    void Camera::SetMovimentSpeed(float speed)
    {
        _movementSpeed = speed;
    }


    /**************************/
    /*     FOLLOW CAMERA      */
    /**************************/

    void FollowCamera::_UpdatePosition(double deltaTime)
    {
        auto cameraPos = _player->GetPosition();

        glm::vec3 playerForward(0.0f);
        auto playerFacingRad = Math::DegToRad(_player->GetRotationAngle());

        playerForward.x = Math::Sin(playerFacingRad);
        playerForward.z = Math::Cos(playerFacingRad);

        cameraPos -= playerForward * _horizontalDistance;
        cameraPos += _worldUp * _verticalDistance;

        _position = cameraPos;
    }

    glm::mat4 FollowCamera::_CalculateViewMatrix()
    {
        return glm::lookAt(_position, _player->GetPosition(), _worldUp);
    }

    void FollowCamera::SetPlayer(GameObject* player)
    {
        _player = player;
    }

    void FollowCamera::SetHorizontalDistance(float horizontalDistance)
    {
        _horizontalDistance = horizontalDistance;
    }

    void FollowCamera::SetVerticalDistance(float verticalDistance)
    {
        _verticalDistance = verticalDistance;
    }
}