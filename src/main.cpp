#include "engine/Game.hpp"

int main(int argc, char const *argv[])
{
    auto *p = new Engine::Game();

    p->Run();

    return 0;
}
