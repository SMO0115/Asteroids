#include "engine/core/CoreModule.h"
#include "game/core/CoreModule.h"


int main(int argc, char* argv[]) {

    Engine engine;
    Asteroids game;

    engine.init("Asteroids", 800, 800, 120);
    engine.run(game);

    return 0;
}
