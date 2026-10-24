//
// Created by marcel on 12/13/25.
//

#pragma once

#include "engine/core/components/BaseComponent.h"

namespace Game::Core {

    // Empty components that only mark what an object is
    // (the ship and the asteroids have components with data: Player::ShipComponent, Asteroid::AsteroidComponent)
    struct BulletTag : Engine::Core::BaseComponent {};
}  // namespace Game::Core
