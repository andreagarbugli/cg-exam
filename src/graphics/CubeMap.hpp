#ifndef ENGINE_CUBEMAP_HPP
#define ENGINE_CUBEMAP_HPP

#include <string>
#include <vector>

#include <SDL_surface.h>

namespace Graphics
{
    class CubeMap
    {
    private:
        unsigned int _textureID;

        SDL_Surface* _image;

    public:
        CubeMap();

        ~CubeMap();

        void Load(const std::vector<std::string>& fileNames);

        void Unload();


        void Disable();

        void Enable();

    };
}

#endif //ENGINE_CUBEMAP_HPP fileNames);

