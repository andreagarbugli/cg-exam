#ifndef ENGINE_CAMERA_HPP
#define ENGINE_CAMERA_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.hpp"
#include "InputManager.hpp"

namespace Engine
{
    class Camera : public GameObject
    {
    private:
        glm::vec3 _front;
        glm::vec3 _up;
        glm::vec3 _right;
        glm::vec3 _worldUp;

        float _yaw;
        float _pitch;

        float _movementSpeed;
        float _turnSpeed;

        glm::mat4 _viewMatrix;

        InputManager* _inputManager = InputManager::GetInstance();

    public:

        Camera() = default;

        Camera(class Game* game, const std::string& name,
               glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
               glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 wordlUp = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = -90.0f,
               float pitch = 0.0f,
               float movementSpeed = 5.0f,
               float turnSpeed = 0.1f) :
                GameObject(game, name, position, scale, rotation),
                _worldUp{ wordlUp },
                _yaw{ yaw },
                _pitch{ pitch },
                _movementSpeed{ movementSpeed },
                _turnSpeed{ turnSpeed },
                _front{ glm::vec3(0.0f, 0.0f, -1.0f) }
        {
        }

        ~Camera() = default;

        void Update(double deltaTime) override;

        glm::mat4 GetViewMatrix() const;

        glm::vec3 GetDirecton() const;

        void SetMovimentSpeed(float speed);

        glm::mat4 _CalculateViewMatrix();

    };
}


#endif //ENGINE_CAMERA_HPP
