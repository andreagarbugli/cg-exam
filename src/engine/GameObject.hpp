#include <utility>

#ifndef ENGINE_GAMEOBJECT_HPP
#define ENGINE_GAMEOBJECT_HPP

#include <vector>

#include <glm/vec3.hpp>

#include "InputManager.hpp"

#include "graphics/Model.hpp"

namespace Engine
{
    enum GameObjectState
    {
        Created,
        Active,
        Paused,
        Remove,
    };

    class GameObject
    {
    protected:
        GameObjectState _state;

        glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 _scale;
        glm::vec3 _rotation;
        glm::vec3 _position;

        float _rotationAngle;

        std::string _name;

        Graphics::Model* _model{};

        class Game* _game;

        glm::mat4 _modelMatrix{};

        InputManager* _inputManager = InputManager::GetInstance();

    public:
        GameObject(class Game* game, std::string name,
                   glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
                   glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f)) :
                _game{game},
                _name{std::move(name)},
                _position{position},
                _scale{scale},
                _rotation{rotation},
                _state{GameObjectState::Created},
                _rotationAngle{0}
        {
        }

        ~GameObject() = default;

        virtual void Draw(Graphics::BasicShader* shader);

        virtual void Update(double deltaTime);

        void SetModel(Graphics::Model* model);

        std::string GetName() const;

        glm::vec3 GetPosition() const;

        void SetPosition(glm::vec3 position);

        float GetRotationAngle() const;

        void SetRotation(float angle, glm::vec3 rotation);

        void SetScale(const glm::vec3& scale);

        GameObjectState GetState() const;

        void SetState(GameObjectState state);
    };
}

#endif //ENGINE_GAMEOBJECT_HPP
