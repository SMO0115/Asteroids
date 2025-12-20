//
// Created by marcel on 11/8/25.
//

#pragma once

#include <map>
#include <string>

#include "engine/core/BaseComponent.h"


struct SoundComponent : public BaseComponent {

    std::map<int, std::string>      sounds;
    int                             current_state   =  0;
    int                             previous_state  = -1;
    int                             channel         = -1;
    bool                            is_looping      = false;
};
