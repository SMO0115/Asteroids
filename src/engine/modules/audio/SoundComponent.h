//
// Created by marcel on 11/8/25.
//

#pragma once

#include <map>
#include <string>

#include "../../core/components/BaseComponent.h"
#include "engine/utils/Types.h"

namespace Engine::Audio {
struct SoundComponent : public Core::BaseComponent {
    std::map<State, std::string>      sounds         = {};
    std::map<State, Assets::AssetID>  sounds_ids     = {};
    State                             current_state  = 0;
    int                               previous_state = -1;
    int                               channel        = -1;
    bool                              is_looping     = false;
};
}  // namespace Engine::Audio