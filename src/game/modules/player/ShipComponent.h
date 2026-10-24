//
// Created by smo0115 on 10/24/26.
//

#pragma once

#include "engine/core/components/BaseComponent.h"

namespace Game::Player {

// Replaces PlayerTag: the ship needs to remember what it's doing, not only THAT it's the ship.
// A tiny state machine; the PlayerSystem moves it from one status to the next.
struct ShipComponent : Engine::Core::BaseComponent {

    enum class Status {
        FLYING,      // normal play
        HYPERSPACE,  // jumped away: invisible (and can't be hit) until it reappears somewhere random
        DESTROYED,   // hit: invisible, waiting to respawn (or the game is over)
    };

    Status status = Status::FLYING;
    float  timer  = 0.0f;  // seconds left in the current status (HYPERSPACE, DESTROYED)
};
}  // namespace Game::Player
