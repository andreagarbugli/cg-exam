#ifndef ENGINE_CAR_HPP
#define ENGINE_CAR_HPP

#include "engine/GameObject.hpp"
#include "engine/InputManager.hpp"

namespace RacingGame
{
    class Car : public Engine::GameObject
    {
    private:

        float accFactor = 0.02f;

        float steerDir = 0.0f;
        float steerFactor = 2.2f; // A
        float steerFriction = 0.93f; // B, sterzo massimo = A*B/(1-B) = 45.17 gradi

        glm::vec3 lightRPosition = glm::vec3(-0.70f, 0.62f, 1.9f);
        glm::vec3 lightLPosition = glm::vec3(0.70f, 0.62f, 1.9f);

        float _grip = 0.45f;

        glm::vec3 _velocityVectorWorld{};
        glm::vec3 _frictionVector = glm::vec3(0.8, 1.0, 0.9991);
        glm::vec3 _velocityVector = glm::vec3(0.0f);

    public:
        Car(Engine::Game* game, std::string name,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f)) :
                Engine::GameObject(game, name, position, scale, rotation)
        {
        }

        void Draw(Graphics::BasicShader* shader) override;

        void Update(double deltaTime) override;

        float GetVelocity();
    };
}

#endif //ENGINE_CAR_HPP
