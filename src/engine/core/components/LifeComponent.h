
#pragma once
#include <glm/vec2.hpp>
#include "BaseComponent.h"

namespace Engine::Core {

struct LifeComponent : public BaseComponent {

    int         max_health      = 0;
    int         current_health  = 0;
    float       life_time       = 0;
};
}