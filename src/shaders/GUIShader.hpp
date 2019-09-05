//
// Created by garbu on 24.08.19.
//

#ifndef ENGINE_GUISHADER_HPP
#define ENGINE_GUISHADER_HPP

#include "BasicShader.hpp"

namespace Graphics
{
    class GUIShader : public BasicShader
    {
    public:

        GUIShader(Engine::Game* game, std::string name);

        void Use() override;
    };
}

#endif //ENGINE_GUISHADER_HPP
