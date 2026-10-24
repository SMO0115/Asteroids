//
// Created by smo0115 on 10/16/26.
//

#pragma once

#include "engine/core/components/BaseComponent.h"

namespace Game::Asteroid {

// Replaces AsteroidTag: an asteroid needs to know its size to know how to split.
// 3 = large, 2 = medium, 1 = small. A hit splits it into two of the next size down;
// a small one just disappears.
struct AsteroidComponent : Engine::Core::BaseComponent {
    int size = 3;
};
}  // namespace Game::Asteroid
