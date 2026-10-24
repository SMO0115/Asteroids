//
// Created by smo0115 on 10/24/26.
//

#pragma once

#include <glm/vec2.hpp>

namespace Game::Events {

// The game's own events, on the engine's EventBus. CombatSystem publishes them; every system that
// cares (score, respawn, sounds, explosions) reads them in the same tick. So CombatSystem doesn't
// need to know who reacts, and adding a reaction doesn't touch CombatSystem.
//
// Values, no GameObject pointers: a reader doesn't have to worry whether the object still exists.

struct AsteroidDestroyedEvent {
    glm::vec2 position;
    int       size;       // 3 = large, 2 = medium, 1 = small
    bool      by_player;  // shot or rammed by the player (only then it scores), not by a saucer
};

struct SaucerDestroyedEvent {
    glm::vec2 position;
    bool      small;
    bool      by_player;
};

struct ShipDestroyedEvent {
    glm::vec2 position;
};
}  // namespace Game::Events
