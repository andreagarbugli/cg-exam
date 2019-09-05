#include "GameObject.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    void GameObject::Draw(Graphics::BasicShader* shader)
    {
        shader->Use();

        _modelMatrix = glm::mat4(1.0f);
        _modelMatrix = glm::translate(_modelMatrix, _position);
        _modelMatrix = glm::scale(_modelMatrix, _scale);
        _modelMatrix = glm::rotate(_modelMatrix, Math::DegToRad(_rotationAngle), _up);

        shader->SetModelMatrix(_modelMatrix);
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

    glm::vec3 GameObject::GetPosition() const
    {
        return _position;
    }

    void GameObject::SetPosition(glm::vec3 position)
    {
        _position = position;
    }

    float GameObject::GetRotationAngle() const
    {
        return _rotationAngle;
    }

    void GameObject::SetRotation(float angle, glm::vec3 rotation)
    {
        _rotationAngle = angle;
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

    void GameObject::SetScale(const glm::vec3& scale)
    {
        _scale = scale;
    }
}