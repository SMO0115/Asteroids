#include "core/CoreModule.h"
#include "engine/core/CoreModule.h"

#include <cstddef>

// The ONE place that decides the screen size. Everything else asks the RenderSystem
// (C++: getScreenSize(), Lua: GetScreenSize()).
constexpr std::size_t SCREEN_WIDTH  = 800;
constexpr std::size_t SCREEN_HEIGHT = 800;


int main(int /*argc*/, char* /*argv*/[]) {

    Engine::Application engine;
    Game::Asteroids     game;

    // No window/renderer/audio = nothing to run on (the error was already printed)
    if (!engine.init("Asteroids", SCREEN_WIDTH, SCREEN_HEIGHT, 120)) return 1;
    engine.run(game);

    return 0;
}
