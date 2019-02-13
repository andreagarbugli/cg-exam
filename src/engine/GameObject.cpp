#include "GameObject.hpp"

namespace Engine
{
    void GameObject::Draw()
    {

    }

    void GameObject::Update(double deltaTime)
    {

    }

    void GameObject::SetModel(Graphics::Model* model)
    {
        _model = model;
    }

    std::string GameObject::GetName() const
    {
        return _name;
    }

    void GameObject::SetName(const std::string& name)
    {
        _name = name;
    }

    glm::vec3 GameObject::GetPosition() const
    {
        return _position;
    }

    void GameObject::SetPosition(glm::vec3 position)
    {
        _position = position;
    }

    glm::vec3 GameObject::GetRotation() const
    {
        return _rotation;
    }

    void GameObject::SetRotation(glm::vec3 rotation)
    {
        _rotation = rotation;
    }

    GameObjectState GameObject::GetState() const
    {
        return _state;
    }

    void GameObject::SetState(GameObjectState state)
    {
        _state = state;
    }

    glm::vec3 GameObject::GetScale() const
    {
        return _scale;
    }

    void GameObject::SetScale(glm::vec3 scale)
    {
        _scale = scale;
    }

}