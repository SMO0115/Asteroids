//
// Created by marcel on 9/29/25.
//

#pragma once
#include <glm/vec2.hpp>
#include "../BaseComponent.h"

struct TransformComponent : public BaseComponent {

    glm::vec2   position;
    float       scale       = 1;
    float       rotation    = 0;
};
