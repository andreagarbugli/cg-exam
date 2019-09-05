#include <glm/gtc/matrix_transform.hpp>

#include "StaticObject.hpp"

#include "engine/Game.hpp"

using namespace std;
using namespace Engine;

namespace RacingGame
{
    void StaticObject::Draw(Graphics::BasicShader* shader)
    {
        GameObject::Draw(shader);

        _model->Draw(shader);
    }
}