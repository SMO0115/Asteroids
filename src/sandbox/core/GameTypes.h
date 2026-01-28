//
// Created by marcel on 11/8/25.
//

#pragma once

namespace Game::Core {
enum class SoundState : int { IDLE = 0, WALK, RUN, JUMP };

enum class AnimationState : int { IDLE = 0, DEATH };

enum class EmittorState : int {

    IDLE = 0,
    INVADER_A,
    INVADER_B,
    INVADER_C,
    MISSILE,
    PLAYER
};

enum class InvaderType : int { INVADER_A, INVADER_B, INVADER_C };
}  // namespace Game::Core