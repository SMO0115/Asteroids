//
// Created by marcel on 11/23/25.
//

#pragma once
#include <cstddef>

#include "engine/core/BaseComponent.h"


namespace Game::Health {
    struct HealthComponent : Engine::Core::BaseComponent {

        std::size_t     max_health;
        std::size_t     current_health;
    };
}