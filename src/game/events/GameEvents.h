//
// Created by marcel on 11/16/25.
//

#pragma once
#include "glm/glm.hpp"

#include "engine/events/EventBus.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "game/core/GameTypes.h"

namespace Engine::Core {
class GameObject;
}

namespace Game::Events {
struct EmitterEvent {
    int                             emitter_animation_id;
    Engine::Core::CollisionLayer layer;
    Engine::Core::CollisionLayer mask;
    glm::vec2                       position;
    glm::vec2                       velocity;
};

struct DeathEvent {
    Engine::Core::GameObject* object;
};

}  // namespace Game::Events