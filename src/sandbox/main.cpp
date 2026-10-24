#include "engine/core/CoreModule.h"
#include "sandbox/core/CoreModule.h"


int main(int /*argc*/, char* /*argv*/[]) {

    Engine::Application engine;
    Game::Sandbox       game;

    if (!engine.init("Sandbox", 800, 800, 120)) return 1;
    engine.run(game);

    return 0;
}
