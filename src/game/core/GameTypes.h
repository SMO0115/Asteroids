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
            WALK,
            RUN,
            JUMP
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

    // 3. Emittor State
    namespace EmittorState {
        enum State : int {
            IDLE = 0,
            INVADER_A,
            INVADER_B,
            INVADER_C,
            MISSILE,
            PLAYER
        };
    }

    // 4. Invader Type
    namespace InvaderType {
        enum Type : int {
            I = 0,
            INVADER_A,
            INVADER_B,
            INVADER_C
        };
    }

    // 5. Game Pools (The most important one for your Scene)
    namespace GamePools {
        enum ID : std::size_t {
            DEFAULT = 0,
            STATIC,
            PLAYER,
            ENEMIES,
            BULLETS,
            UI,

            COUNT // Automatically 5, used for array sizing
        };
    }

    struct Entities {};
}  // namespace Game::Core