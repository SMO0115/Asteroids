#include "core/CoreModule.h"
#include "engine/core/CoreModule.h"

int main(int argc, char* argv[]) {
    Engine::Application engine;
    Game::Asteroids     game;

    engine.init("Asteroids", 800, 800, 120);
    engine.run(game);

    return 0;
}
