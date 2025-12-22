//
// Created by marcel on 11/23/25.
//

#pragma once
#include <cstddef>

#include "engine/core/BaseComponent.h"


namespace Game::Health {
    struct LifetimeComponent : public Engine::Core::BaseComponent {

        float       time_left;

        LifetimeComponent(float time) : time_left(time) {}
    };
}