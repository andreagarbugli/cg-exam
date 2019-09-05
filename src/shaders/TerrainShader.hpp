#ifndef ENGINE_TERRAINSHADER_HPP
#define ENGINE_TERRAINSHADER_HPP

#include "BasicShader.hpp"

namespace Graphics
{
    class TerrainShader : public BasicShader
    {
    public:
        TerrainShader(Engine::Game* game, std::string name);

        void Use() override;
    };
}

#endif //ENGINE_TERRAINSHADER_HPP
