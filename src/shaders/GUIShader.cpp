//
// Created by garbu on 24.08.19.
//

#include "GUIShader.hpp"

#include "engine/Game.hpp"

namespace Graphics
{
    GUIShader::GUIShader(Engine::Game* game, std::string name)
            : BasicShader(game, name)
    {
        _shaderProgram = new ShaderProgram();
        _shaderProgram->Load("shaders/sprite.vert", "shaders/sprite.frag");
    }

    void GUIShader::Use()
    {
        BasicShader::Use();

        _shaderProgram->SetUniformInt("guiTexture", 0);

    }
}

