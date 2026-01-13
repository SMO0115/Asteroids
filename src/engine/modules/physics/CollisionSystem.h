//
// Created by marcel on 11/22/25.
//

#pragma once

#include <memory>
#include <vector>
#include "engine/events/EngineEvents.h"

namespace Engine::Core {
class GameObject;
class TransformComponent;
struct Rect;
}  // namespace Engine::Core

namespace Engine::Physics {
class CollisionSystem {
public:
    CollisionSystem()  = default;
    ~CollisionSystem() = default;

    void update(Events::EngineEventBus& event_bus, const std::vector<std::unique_ptr<Core::GameObject> >& game_objects);


    bool checkCollisionAABB_(const Core::TransformComponent& trans_a, const Core::TransformComponent& trans_b, const Core::Rect& obj_a,
                             const Core::Rect& obj_b);
};
}  // namespace Engine::Physics