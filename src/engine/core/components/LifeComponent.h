
#pragma once
#include <glm/vec2.hpp>
#include "BaseComponent.h"

namespace Engine::Core {

struct LifeComponent : public BaseComponent {

    int         max_health        = 0;
    int         current_health    = 0;
    float       life_time         = 0;

    // When life_time runs out: false = switch off (pooled objects like bullets are reused),
    // true = destroy (one-shot objects like explosion debris, which would otherwise pile up)
    bool        destroy_on_expire = false;
};
}