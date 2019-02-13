#include <iostream>

#include <GL/glew.h>

#include <SDL2/SDL_image.h>

#include "Texture.hpp"

namespace Graphics
{

    Texture::Texture() :
            _textureID{ 0 }
    {
    }

    Texture::~Texture()
    {
        Unload();
    }

    void Texture::Load(const std::string& fileName)
    {
        _image = IMG_Load(fileName.c_str());

        if (!_image)
        {
            std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
        }

        GLenum format = GL_RGB;
        if (_image->format->BytesPerPixel == 4)
        {
            format = GL_RGBA;
        }

        glGenTextures(1, &_textureID);
        glBindTexture(GL_TEXTURE_2D, _textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        _width = _image->w;
        _height = _image->h;

        glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height,
                     0, format, GL_UNSIGNED_BYTE, _image->pixels);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);

        glBindTexture(GL_TEXTURE_2D, 0);

        IMG_Quit();
    }

    void Texture::Enable(int number)
    {
        glActiveTexture(GL_TEXTURE0 + number);
        glBindTexture(GL_TEXTURE_2D, _textureID);
    }

    void Texture::Disable()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::Unload()
    {
        glDeleteTextures(1, &_textureID);
    }

    int Texture::GetWidth() const
    {
        return _width;
    }

    int Texture::GetHeight() const
    {
        return _width;
    }

}