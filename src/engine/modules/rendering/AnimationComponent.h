//
// Created by marcel on 10/31/25.
//

#pragma once

#include <map>
#include <string>
#include "engine/core/components/BaseComponent.h"

namespace Engine::Assets {
struct Animation;
}

namespace Engine::Graphics {
    struct AnimationComponent : public Core::BaseComponent {

        std::map<std::string, std::string>        animations_ids = {};
        std::map<std::size_t, Assets::Animation*> animations     = {};

        std::size_t current_state    = 0;
        std::size_t current_frame    = 0;
        float       current_duration = 0.0f;
        float       speed            = 1.0f;
    };
}  // namespace Engine::Graphics