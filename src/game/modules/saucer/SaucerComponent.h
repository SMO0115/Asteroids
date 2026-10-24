//
// Created by smo0115 on 10/24/26.
//

#pragma once

#include "engine/core/components/BaseComponent.h"

namespace Game::Saucer {

// A flying saucer (the arcade's UFO). The SaucerSystem flies it across the screen once, zig-zagging
// and shooting; then it leaves.
struct SaucerComponent : Engine::Core::BaseComponent {
    bool  small      = false;  // small: aims at the ship, 1000 points; large: shoots anywhere, 200 points
    float travelled  = 0.0f;   // px flown left-right; it leaves after crossing the screen once
    float fire_timer = 1.0f;   // seconds to the next shot (not 0: no shot the moment it appears)
    float turn_timer = 1.0f;   // seconds to the next zig-zag
};
}  // namespace Game::Saucer
