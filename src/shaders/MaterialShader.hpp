#ifndef ENGINE_MATERIALSHADER_HPP
#define ENGINE_MATERIALSHADER_HPP

#include <string>

#include "BasicShader.hpp"

#include "graphics/ShaderProgram.hpp"

namespace Graphics
{
    class MaterialShader : public BasicShader
    {
    public:
        MaterialShader(Engine::Game* game, std::string name);

        void Use() override;

        void SetModel(class SubModel* model) override;
    };
}

#endif //ENGINE_MATERIALSHADER_HPP
