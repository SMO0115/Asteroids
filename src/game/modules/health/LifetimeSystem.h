//
// Created by marcel on 12/6/25.
//

#pragma once


#include <vector>
#include <memory>
#include "../../events/GameEvents.h"


class GameObject;

class LifetimeSystem {
public:
    LifetimeSystem() = default;
    ~LifetimeSystem() = default;

    void update(float deltaTime, GameEventBus& event_bus, std::vector< std::unique_ptr<GameObject> > &game_objects);
};
