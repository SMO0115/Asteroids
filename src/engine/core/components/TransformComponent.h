//
// Created by marcel on 9/29/25.
//

#pragma once
#include <glm/vec2.hpp>
#include "BaseComponent.h"

namespace Engine::Core {
    struct TransformComponent : public BaseComponent {

        glm::vec2   position    = glm::vec2(0.f, 0.f);
        float       scale       = 1;
        float       rotation    = 0;
    };
}