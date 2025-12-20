//
// Created by marcel on 11/17/25.
//

#pragma once
#include "engine/core/BaseComponent.h"

struct EmitterComponent : public BaseComponent {

    float       launch_speed    = 500.0f;
    float       cooldown        = 5.0f;
    float       cooldown_timer  = 0.0f;
    bool        active          = false;
};