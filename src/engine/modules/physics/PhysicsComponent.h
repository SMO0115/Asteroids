//
// Created by marcel on 9/29/25.
//

#pragma once

#include <glm/vec2.hpp>

#include "../../core/components/BaseComponent.h"

namespace Engine::Physics {
// Every member has a default: glm::vec2 and float are NOT zeroed on their own,
// so 'PhysicsComponent p;' would otherwise start with garbage
struct PhysicsComponent : public Core::BaseComponent {
    glm::vec2 velocity            = {0.f, 0.f};
    glm::vec2 acceleration        = {0.f, 0.f};
    float     damping             = 1.0f;  // fraction of velocity kept per second (1 = none lost)
    float     angularVelocity     = 0.0f;
    float     angularAcceleration = 0.0f;
    float     angularDamping      = 1.0f;
    float     mass                = 1.0f;
};
}  // namespace Engine::Physics
