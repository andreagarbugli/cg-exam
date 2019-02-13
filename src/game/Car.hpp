#ifndef ENGINE_CAR_HPP
#define ENGINE_CAR_HPP

#include "engine/GameObject.hpp"
#include "engine/InputManager.hpp"

using namespace Engine;

namespace RacingGame
{
    class Car : public GameObject
    {
    private:
        glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);

        float accFactor = 0.02f;

        const float GRAVITY = -0.00981f;
        const float TERRAIN_HEIGHT = 0.0f;

        bool _isInAir = false;

        float steerDir = 0.0f;
        float steerFactor = 3.2f; // A
        float steerFriction = 0.93f; // B, sterzo massimo = A*B/(1-B) = 45.17 gradi

        float _facing = 0.0f;
        float _grip = 0.45f;

        glm::vec3 _frictionVector = glm::vec3(0.8, 1.0, 0.9991);
        glm::vec3 _velocityVector = glm::vec3(0.0f);

        InputManager* _inputManager = InputManager::GetInstance();

    public:
        Car(class Game* game, const std::string& name,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f)) :
                GameObject(game, name, position, scale, rotation)
        {
        }

        void Draw() override;

        void Update(double deltaTime) override;
    };
}

#endif //ENGINE_CAR_HPP
