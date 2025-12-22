//
// Created by marcel on 10/31/25.
//

#pragma once

#include <map>

#include "engine/core/BaseComponent.h"


namespace Engine::Assets { struct Animation; }


namespace Engine::Graphics {
    struct AnimationComponent : public Core::BaseComponent {

        std::map<std::size_t , Assets::Animation*>      animations;
        std::size_t                                     current_state;
        std::size_t                                     current_frame       = 0;
        float                                           current_duration    = 0.0f;
        float                                           speed               = 1.0f;
    };
}