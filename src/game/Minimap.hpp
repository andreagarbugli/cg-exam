//
// Created by garbu on 23.08.19.
//

#ifndef ENGINE_MINIMAP_HPP
#define ENGINE_MINIMAP_HPP

#include <utility>

#include "graphics/GUIComponent.hpp"

namespace RacingGame
{
    class Minimap : public Graphics::GUIComponent
    {
    private:

        unsigned int _framebuffer;
        unsigned int _renderbuffer;

        float _ratio;

    public:

        Minimap(Engine::Game* game, const std::string& name);

        bool CreateMinimapTarget();

        void Draw(Graphics::BasicShader* shader) override;

        float GetRatio() const;

        void SetRatio(float ratio);

        unsigned int GetFrameBuffer();


    };
}


#endif //ENGINE_MINIMAP_HPP
