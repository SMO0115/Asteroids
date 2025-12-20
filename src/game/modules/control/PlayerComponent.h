//
// Created by marcel on 11/22/25.
//

# pragma once

#include "engine/core/CoreModule.h"

struct PlayerComponent : public BaseComponent {


    float       shoot_cooldown          = 0.5f;
    float       shoot_cooldown_timer    = 0.0f;
};