//
// Created by marcel on 9/29/25.
//

#pragma once

#include <cstdint>
#include <glm/vec2.hpp>

#include "../../core/components/BaseComponent.h"

namespace Engine::Physics {

    // A set of collision layers, one bit per layer. The engine doesn't know what the bits MEAN:
    // the game names them (Asteroids: PLAYER, ASTEROID, PLAYER_BULLET in game/core/GameTypes.h).
    using CollisionMask = std::uint32_t;

    // A circle collider. A circle looks the same at every rotation, so it fits spinning sprites
    // (the old axis-aligned box didn't turn with the asteroid and stuck out past its corners).
    struct ColliderComponent : public Core::BaseComponent {
        CollisionMask layer = 0;  // the layer(s) this object is on
        CollisionMask mask  = 0;  // the layers it collides with

        float     radius = 0.0f;          // in pixels, multiplied by TransformComponent::scale
        glm::vec2 offset = {0.0f, 0.0f};  // circle center relative to the position; turns with the object
    };
}  // namespace Engine::Physics
