#include <iostream>
#include <SDL2/SDL.h>

#include "engine/Game.hpp"

int main(int argc, char const *argv[])
{
    Engine::Game *p = new Engine::Game();

    p->Run();

    return 0;
}
