//
// Created by marcel on 9/29/25.
//

#pragma once

#include <glm/vec2.hpp>

#include "../../core/components/BaseComponent.h"

namespace Engine::Physics {
struct PhysicsComponent : public Core::BaseComponent {
    glm::vec2 velocity;
    glm::vec2 acceleration;
    float damping;
    float angularVelocity;
    float angularAcceleration;
    float angularDamping;
    float     mass;
};
}  // namespace Engine::Physics