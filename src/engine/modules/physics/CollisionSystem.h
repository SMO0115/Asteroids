//
// Created by marcel on 11/22/25.
//

#pragma once

#include <memory>
#include <vector>
#include "engine/events/EngineEvents.h"

namespace Engine::Core { class GameObject; class TransformComponent; struct Rect; class Context; }  // namespace Engine::Core

namespace Engine::Core {
    class CollisionSystem {
    public:
        CollisionSystem()  = default;
        ~CollisionSystem() = default;

        void update(Core::Context& ctx);


        bool checkCollisionAABB_(const Core::TransformComponent& trans_a, const Core::TransformComponent& trans_b, const Core::Rect& obj_a,
                                 const Core::Rect& obj_b);
    };
}  // namespace Engine::Physics