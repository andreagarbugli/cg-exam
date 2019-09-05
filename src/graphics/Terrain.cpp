#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <SDL_image.h>

#include "Terrain.hpp"

#include "engine/Game.hpp"

using namespace std;
using namespace Engine;

namespace Graphics
{
    Mesh* Terrain::_GenerateTerrain()
    {
        auto heightMap = IMG_Load("textures/heightmap.png");

        if (!heightMap)
        {
            cerr << "IMG_Load: " << IMG_GetError() << endl;
        }

        _vertexCount = heightMap->h;

        terrainHeights = new float[_vertexCount * _vertexCount];

        auto rawMesh = new Utils::RawMesh();

        for (int i = 0; i < _vertexCount; ++i)
        {
            for (int j = 0; j < _vertexCount; ++j)
            {
                float x = j / (_vertexCount - 1.0f);
                float z = i / (_vertexCount - 1.0f);
                float y = 0;

                if (_withHeightMap)
                    y = _GetHeight(heightMap, i, j);

                rawMesh->vertices.emplace_back(x * SIZE, y, z * SIZE);
                rawMesh->uvs.emplace_back(x, z);
                rawMesh->normals.emplace_back(_CalculateNormal(heightMap, i, j));

                terrainHeights[i * _vertexCount + j] = y;
            }
        }

        for (int gz = 0; gz < _vertexCount - 1; gz++)
        {
            for (int gx = 0; gx < _vertexCount - 1; gx++)
            {
                unsigned int topLeft = (gz * _vertexCount) + gx;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = ((gz + 1) * _vertexCount) + gx;
                unsigned int bottomRight = bottomLeft + 1;
                rawMesh->indices.push_back(topLeft);
                rawMesh->indices.push_back(bottomLeft);
                rawMesh->indices.push_back(topRight);
                rawMesh->indices.push_back(topRight);
                rawMesh->indices.push_back(bottomLeft);
                rawMesh->indices.push_back(bottomRight);
            }
        }

        return new Mesh(*rawMesh);

    }

    float Terrain::_GetHeight(SDL_Surface* image, int x, int y)
    {
        if (x < 0 || x > image->h || y < 0 || y > image->h)
        {
            return 0;
        }

        SDL_LockSurface(image);
        auto pixel = _GetPixel(image, x, y);
        SDL_UnlockSurface(image);

        auto height = (float) (pixel & 0xff);
        height -= MAX_PIXEL_COLOR / 2.0f;
        height /= MAX_PIXEL_COLOR / 2.0f;
        height *= MAX_HEIGHT;

        return height;
    }

    glm::vec3 Terrain::_CalculateNormal(SDL_Surface* image, int x, int y)
    {
        float heightL = _GetHeight(image, x - 1, y);
        float heightR = _GetHeight(image, x + 1, y);
        float heightD = _GetHeight(image, x, y - 1);
        float heightU = _GetHeight(image, x, y + 1);

        glm::vec3 normal = glm::vec3(heightL - heightR, 2.0f, heightD - heightU);

        return glm::normalize(normal);
    }

    Uint32 Terrain::_GetPixel(SDL_Surface* image, int x, int y)
    {

        int bpp = image->format->BytesPerPixel;
        /* Here p is the address to the pixel we want to retrieve */
        Uint8* p = (Uint8*) image->pixels + y * image->pitch + x * bpp;

        switch (bpp)
        {
            case 1:
                return *p;
                break;

            case 2:
                return *(Uint16*) p;
                break;

            case 3:
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                    return p[0] << 16 | p[1] << 8 | p[2];
                else
                    return p[0] | p[1] << 8 | p[2] << 16;
                break;

            case 4:
                return *(Uint32*) p;
                break;

            default:
                return 0;       /* shouldn't happen, but avoids warnings */
        }
    }

    void Terrain::Draw(BasicShader* shader)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(_x, 0, _z));

        _back->Enable(0);
        _rock->Enable(1);
        _street->Enable(2);
        _blendMap->Enable(3);
        _specular->Enable(4);

        shader->Use();

        shader->SetModelMatrix(modelMatrix);

        _mesh->Use();
    }

    void Terrain::SetShader(BasicShader* shader)
    {
        _shader = shader;
    }
}