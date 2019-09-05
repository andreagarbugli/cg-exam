#ifndef ENGINE_TEXTURE_HPP
#define ENGINE_TEXTURE_HPP

#include <string>

#include <SDL2/SDL.h>

namespace Graphics
{
    class Texture
    {
    private:
        unsigned int _textureID;

        SDL_Surface* _image{};

        int _width;
        int _height;

    public:
        Texture();

        ~Texture();


        void Load(const std::string& fileName);

        void Unload();


        void Disable();

        void Enable(int number);


        int GetHeight() const;

        unsigned int GetTextureID();

        int GetWidth() const;


        void CreateForRendering(int width, int height, unsigned int format);

        void CreateFromSurace(SDL_Surface* surface);
    };
}

#endif //ENGINE_TEXTURE_HPP
