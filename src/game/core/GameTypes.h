//
// Created by marcel on 11/8/25.
//

#pragma once


enum class SoundState : int{
    IDLE = 0,
    WALK,
    RUN,
    JUMP
};


enum class AnimationState : int {
    IDLE = 0,
    DEATH
};



enum class EmittorState : int {

    IDLE = 0,
    INVADER_A,
    INVADER_B,
    INVADER_C,
    MISSILE,
    PLAYER
};

enum class CollisionLayer : int {

    NONE            = 0,
    PLAYER          = 1 << 0,
    INVADER         = 1 << 1,
    PLAYER_BULLET   = 1 << 2,
    INVADER_BULLET  = 1 << 3,
    WALL            = 1 << 4
};

inline CollisionLayer operator|(CollisionLayer a, CollisionLayer b) {
    return static_cast<CollisionLayer>(static_cast<std::uint16_t>(a) | static_cast<std::uint16_t>(b));
}

inline CollisionLayer operator&(CollisionLayer a, CollisionLayer b) {
    return static_cast<CollisionLayer>(static_cast<std::uint16_t>(a) & static_cast<std::uint16_t>(b));
}

enum class InvaderType : int {
    INVADER_A,
    INVADER_B,
    INVADER_C
};
