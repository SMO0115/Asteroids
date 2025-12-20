//
// Created by marcel on 11/8/25.
//

#pragma once

#include "engine/core/BaseComponent.h"

struct AIComponent : public BaseComponent {

    int         currentState    = 1;
    int         stateRepetition = 0;
    float       stateTimer      = 0.0f;
    float       stateLimit      = 1.0f;
    float       currentSpeed    = 20.0f;
};