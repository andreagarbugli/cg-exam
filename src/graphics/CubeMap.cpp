#include <iostream>

#include <GL/gl3w.h>

#include <SDL2/SDL_image.h>

#include "CubeMap.hpp"

using namespace std;

namespace Graphics
{

CubeMap::CubeMap() : _textureID{0}
{
}

CubeMap::~CubeMap()
{
    Unload();
}

void CubeMap::Load(const vector<string> &fileNames)
{
    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);

    for (int i = 0; i < fileNames.size(); ++i)
    {
        _image = IMG_Load(fileNames[i].c_str());

        if (!_image)
        {
            std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
        }

        GLenum format = GL_RGB;
        if (_image->format->BytesPerPixel == 4)
        {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, _image->w, _image->h,
                     0, format, GL_UNSIGNED_BYTE, _image->pixels);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Disable();

    IMG_Quit();
}

void CubeMap::Enable()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
}

void CubeMap::Disable()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::Unload()
{
    glDeleteTextures(1, &_textureID);
}
} // namespace Graphics