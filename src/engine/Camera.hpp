#ifndef ENGINE_CAMERA_HPP
#define ENGINE_CAMERA_HPP

#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObject.hpp"
#include "InputManager.hpp"

namespace Engine
{
    /**
     * Basic Camera
     */
    class BasicCamera : public GameObject
    {
    protected:
        glm::vec3 _front;
        glm::vec3 _right{};
        glm::vec3 _worldUp;

        glm::mat4 _viewMatrix{};

    public:
        BasicCamera(class Game* game, const std::string& name,
                    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
                    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 wordlUp = glm::vec3(0.0f, 1.0f, 0.0f)) :
                GameObject(game, name, position, scale, rotation),
                _worldUp{wordlUp},
                _front{glm::vec3(0.0f, 0.0f, 1.0f)}
        {
        }

        ~BasicCamera() = default;


        void Update(double deltaTime) override;


        glm::mat4 GetViewMatrix() const;

        glm::vec3 GetDirecton() const;

        glm::vec3 GetFront();

    protected:

        virtual void _UpdatePosition(double deltaTime);

        virtual glm::mat4 _CalculateViewMatrix();
    };

    /**
     * Camera
     */
    class Camera : public BasicCamera
    {
    private:
        float _yaw;
        float _pitch;

        float _movementSpeed;
        float _turnSpeed;

        InputManager* _inputManager = InputManager::GetInstance();

    public:

        Camera(class Game* game, const std::string& name,
               glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
               glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 wordlUp = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = -90.0f,
               float pitch = 0.0f,
               float movementSpeed = 5.0f,
               float turnSpeed = 0.1f) :
                BasicCamera(game, name, position, scale, rotation, wordlUp),
                _yaw{yaw},
                _pitch{pitch},
                _movementSpeed{movementSpeed},
                _turnSpeed{turnSpeed}
        {
        }

        ~Camera() = default;


        void SetMovimentSpeed(float speed);

    protected:

        void _UpdatePosition(double deltaTime) override;
    };


    /**
     * Follow Camera
     */
    class FollowCamera : public BasicCamera
    {
    private:

        float _horizontalDistance;
        float _verticalDistance;

        GameObject* _player = nullptr;

    public:

        FollowCamera(class Game* game, const std::string& name,
                     glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
                     glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3 wordlUp = glm::vec3(0.0f, 1.0f, 0.0f)) :
                BasicCamera(game, name, position, scale, rotation, wordlUp),
                _horizontalDistance{12.0f},
                _verticalDistance{3.0f}
        {
        }

        ~FollowCamera() = default;


        void SetHorizontalDistance(float horizontalDistance);

        void SetVerticalDistance(float verticalDistance);

        void SetPlayer(GameObject* player);

    protected:

        glm::mat4 _CalculateViewMatrix() override;

        void _UpdatePosition(double deltaTime) override;

    };
}


#endif //ENGINE_CAMERA_HPP
