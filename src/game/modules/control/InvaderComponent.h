//
// Created by marcel on 11/22/25.
//

#pragma once

#include "engine/core/CoreModule.h"
#include "../../core/GameTypes.h"


struct InvaderComponent : public BaseComponent {
    InvaderType     type;
    int             score_value = 10;
};