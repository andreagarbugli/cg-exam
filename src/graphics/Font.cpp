//
// Created by garbu on 23.08.19.
//

#include <vector>
#include <glm/detail/type_vec3.hpp>
#include <iostream>
#include "Font.hpp"

namespace Graphics
{
    bool Font::Load(const std::string& fileName)
    {
        std::vector<int> fontSizes = {
                8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28,
                30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 52, 56,
                60, 64, 68, 72
        };

        for (auto& size : fontSizes)
        {
            TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
            if (font == nullptr)
            {
                return false;
            }

            _fontData.emplace(size, font);
        }

        return true;
    }

    Texture* Font::RenderText(const std::string& text, const glm::vec3& color, int pointSize)
    {
        Texture* texture = nullptr;

        SDL_Color sdlColor;
        sdlColor.r = static_cast<u_int8_t>(color.r * 255);
        sdlColor.g = static_cast<u_int8_t>(color.g * 255);
        sdlColor.b = static_cast<u_int8_t>(color.b * 255);
        sdlColor.a = 255;

        auto iter = _fontData.find(pointSize);
        if (iter != _fontData.end())
        {
            TTF_Font* font = iter->second;
            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), sdlColor);

            if (surface != nullptr)
            {
                texture = new Texture();
                texture->CreateFromSurace(surface);

                SDL_FreeSurface(surface);
            }
        }

        return texture;
    }

    void Font::Unload()
    {
        for (auto& font : _fontData)
        {
            TTF_CloseFont(font.second);
        }
    }

    TTF_Font* Font::GetTTFFont(int index)
    {
        TTF_Font* result = nullptr;

        auto iter = _fontData.find(index);
        if (iter != _fontData.end())
        {
            result = iter->second;
        }

        return result;
    }
}
