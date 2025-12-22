//
// Created by marcel on 9/29/25.
//

#pragma once

#include "engine/core/BaseComponent.h"
#include "engine/utils/Types.h"


namespace Engine::Core          { class GameObject; }


namespace Engine::Physics {
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


    struct ColliderComponent : public Core::BaseComponent {

        CollisionLayer      layer   = CollisionLayer::NONE;
        CollisionLayer      mask    = CollisionLayer::NONE;

        Core::Rect          bounds  = {0, 0, 0, 0};
    };
}