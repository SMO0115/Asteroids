//
// Created by marcel on 11/23/25.
//

#pragma once

#include <vector>
#include <memory>
#include "../../../engine/events/EngineEvents.h"

class GameObject;

class HealthSystem {
public:
    HealthSystem() = default;
    ~HealthSystem() = default;

    void update(float deltaTime, EngineEventBus& event_bus, std::vector<std::unique_ptr<GameObject> > &game_objects);
};
