#ifndef ENGINE_BASICSHADER_HPP
#define ENGINE_BASICSHADER_HPP

#include <string>
#include <utility>

#include "graphics/ShaderProgram.hpp"

namespace Engine
{
    class Game;
}

namespace Graphics
{
    class BasicShader
    {
    protected:
        ShaderProgram* _shaderProgram;

        std::string _name;

        Engine::Game* _game;

    public:
        BasicShader(Engine::Game* game, std::string& name) :
                _game{game},
                _name{name}
        {
        }

        BasicShader(Engine::Game* game, ShaderProgram* shaderProgram) :
                _game{game},
                _shaderProgram{shaderProgram}
        {
        }

        virtual void Use();

        std::string GetName();

        ShaderProgram* GetShaderProgram();

        virtual void SetModel(class SubModel* model);

        void SetModelMatrix(glm::mat4 modelMatrix);

        void UnUse();
    };
}

#endif //ENGINE_BASICSHADER_HPP
