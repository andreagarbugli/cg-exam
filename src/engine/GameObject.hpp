#include <utility>

#ifndef ENGINE_GAMEOBJECT_HPP
#define ENGINE_GAMEOBJECT_HPP

#include <vector>

#include <glm/vec3.hpp>
#include <graphics/Model.hpp>

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

        glm::vec3 _scale;
        glm::vec3 _rotation;
        glm::vec3 _position;

        std::string _name;

        Graphics::Model* _model{ };

        class Game* _game;

    public:
        GameObject(class Game* game, std::string name,
                   glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f)) :
                _game{ game },
                _name{ std::move(name) },
                _position{ position },
                _scale{ scale },
                _rotation{ rotation },
                _state{ GameObjectState::Created }
        {
        }

        ~GameObject() = default;

        virtual void Draw();

        virtual void Update(double deltaTime);

        void SetModel(Graphics::Model* model);

        std::string GetName() const;

        void SetName(const std::string& name);

        glm::vec3 GetPosition() const;

        void SetPosition(glm::vec3 position);

        glm::vec3 GetRotation() const;

        void SetRotation(glm::vec3 rotation);

        GameObjectState GetState() const;

        void SetState(GameObjectState state);

        glm::vec3 GetScale() const;

        void SetScale(glm::vec3 scale);

    };
}

#endif //ENGINE_GAMEOBJECT_HPP
