//
// Created by marcel on 9/29/25.
//

#pragma once

#include <glm/vec2.hpp>

#include "engine/core/BaseComponent.h"


struct PhysicsComponent : public BaseComponent {

    glm::vec2           velocity;
    glm::vec2           angularVelocity;
    glm::vec2           acceleration;
    glm::vec2           angularDamping;
    float               mass;
};
