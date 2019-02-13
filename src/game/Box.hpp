#ifndef ENGINE_BOX_HPP
#define ENGINE_BOX_HPP

#include "engine/GameObject.hpp"

using namespace Engine;

namespace Game
{
    class Box : public GameObject
    {
    public:
        Box(class Game* game, const std::string& name) :
                GameObject(game, name)
        {
        }
    };
}

#endif //ENGINE_BOX_HPP
