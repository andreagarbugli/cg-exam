//
// Created by andrea on 08/02/19.
//

#ifndef ENGINE_TERRAIN_HPP
#define ENGINE_TERRAIN_HPP

#include <SDL2/SDL_image.h>

#include "Mesh.hpp"
#include "Texture.hpp"
#include "GLSLShader.hpp"

#include "engine/GameObject.hpp"

using namespace Engine;

namespace Graphics
{
    class Terrain : public GameObject
    {
    private:
        static constexpr float SIZE = 800.0f;
        static constexpr float MAX_HEIGHT = 40.0f;
        static constexpr int MAX_PIXEL_COLOR  = 256;

        float* terrainHeights;

        float _x;
        float _z;

        bool _withHeightMap = false;

        int _vertexCount;

        Mesh* _mesh;
        Texture* _back;
        Texture* _rock;
        Texture* _street;
        Texture* _blendMap;

        Texture* _specular;

        GLSLShader* _shader;

    public:
        Terrain(class Game* game, const std::string& name,
                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
                int gridX = -1,
                int gridZ = -1) :
                GameObject(game, name, position, scale, rotation),
                _x{ gridX * SIZE / 2},
                _z{ gridZ * SIZE / 2}
        {
            _mesh = _GenerateTerrain();

            _back = new Texture();
            _back->Load("textures/grass.jpg");

            _rock = new Texture();
            _rock->Load("textures/stones.png");

            _street = new Texture();
            _street->Load("textures/gravel.jpg");

            _blendMap = new Texture();
            _blendMap->Load("textures/blendMap.jpg");

            _specular = new Texture();
            _specular->Load("textures/black.jpg");
        }

        void Draw() override;

        float GetTerrainHeightAt(float worldX, float worldZ);

        void SetShader(GLSLShader* shader);

     //   void SetTexture(Texture* texture);

    private:

        glm::vec3 _CalculateNormal(SDL_Surface* image, int x, int y);

        Mesh* _GenerateTerrain();

        float _GetHeight(SDL_Surface* image,int x, int y);

        Uint32 _GetPixel(SDL_Surface* image, int x, int y);
    };
}

#endif //ENGINE_TERRAIN_HPP
