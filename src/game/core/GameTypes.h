//
// Created by marcel on 11/8/25.
//

#pragma once

#include <cstddef> // for std::size_t

namespace Game::Core {

    // 1. Sound State
    namespace SoundState {
        enum State : int {
            IDLE = 0,
            RUN
        };
    }

    // 2. Animation State
    namespace AnimationState {
        enum State : int {
            IDLE = 0,
            DEATH,
            RUN
        };
    }

    // 3. Collision Layers: one bit each. The engine only sees a bit mask (Engine::Physics::CollisionMask);
    // this is where the bits get their meaning. Lua uses these names (registered in ComponentBinding.cpp).
    namespace CollisionLayer {
        enum Layer : int {
            NONE          = 0,
            PLAYER        = 1 << 0,
            ASTEROID      = 1 << 1,
            PLAYER_BULLET = 1 << 2,
            SAUCER        = 1 << 3,
            SAUCER_BULLET = 1 << 4
        };
    }

    // 4. Game Pools (The most important one for your Scene)
    namespace GamePools {
        enum ID : std::size_t {
            DEFAULT = 0,
            STATIC,
            PLAYER,
            ENEMIES,
            BULLETS,
            SAUCER_BULLETS,
            EFFECTS,  // explosion debris
            UI,

            COUNT // Automatically 8, used for array sizing
        };
    }
}  // namespace Game::Core
