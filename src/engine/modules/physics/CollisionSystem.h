//
// Created by marcel on 11/22/25.
//

#pragma once

#include <glm/vec2.hpp>
#include "engine/events/EngineEvents.h"

namespace Engine::Core { class Context; }  // namespace Engine::Core

namespace Engine::Physics {
    class CollisionSystem {
    public:
        CollisionSystem()  = default;
        ~CollisionSystem() = default;

        void update(Core::Context& ctx);

        // Two circles overlap when their centers are closer than their radii added up.
        // Static (it needs no state) and public, so the tests can call it directly.
        static bool circlesOverlap(glm::vec2 center_a, float radius_a, glm::vec2 center_b, float radius_b);
    };
}  // namespace Engine::Physics
