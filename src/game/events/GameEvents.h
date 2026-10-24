//
// Created by smo0115 on 10/24/26.
//

#pragma once

#include <glm/vec2.hpp>

namespace Game::Events {

// The game's own events, on the engine's EventBus. AsteroidSystem publishes them; every system that
// cares (score, respawn, sounds, explosions) reads them in the same tick. So AsteroidSystem doesn't
// need to know who reacts, and adding a reaction doesn't touch AsteroidSystem.
//
// Values, no GameObject pointers: a reader doesn't have to worry whether the object still exists.

struct AsteroidDestroyedEvent {
    glm::vec2 position;
    int       size;  // 3 = large, 2 = medium, 1 = small
};

struct ShipDestroyedEvent {
    glm::vec2 position;
};
}  // namespace Game::Events
