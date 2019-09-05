//
// Created by garbu on 23.08.19.
//

#ifndef ENGINE_FONT_HPP
#define ENGINE_FONT_HPP

#include <unordered_map>

#include <SDL2/SDL_ttf.h>
#include <glm/fwd.hpp>
#include "Texture.hpp"

namespace Graphics
{
    class Font
    {
    private:

        std::unordered_map<int, TTF_Font*> _fontData;

    public:

        Font() = default;

        ~Font() = default;

        bool Load(const std::string& fileName);

        void Unload();

        Texture* RenderText(const std::string& text,
                            const glm::vec3& color,
                            int pointSize = 30);

        TTF_Font* GetTTFFont(int index);

    };
}

#endif //ENGINE_FONT_HPP
