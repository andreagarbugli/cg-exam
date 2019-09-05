#ifndef ENGINE_STATICOBJECT_HPP
#define ENGINE_STATICOBJECT_HPP

#include <glm/glm.hpp>

#include "engine/GameObject.hpp"
#include "engine/InputManager.hpp"

namespace RacingGame
{
    class StaticObject : public Engine::GameObject
    {
    public:
        StaticObject(Engine::Game* game, const std::string& name,
                     glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
                     glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f)) :
                Engine::GameObject(game, name, position, scale, rotation)
        {
        }

        void Draw(Graphics::BasicShader* shader) override;
    };
}

#endif //ENGINE_STATICOBJECT_HPP
