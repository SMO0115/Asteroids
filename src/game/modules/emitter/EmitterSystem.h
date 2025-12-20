//
// Created by marcel on 11/17/25.
//

#pragma once
#include <vector>
#include <memory>
#include "../../events/GameEvents.h"

class GameObject;

class EmitterSystem {

public:

    EmitterSystem() = default;
    ~EmitterSystem() = default;

    void update(float deltaTime, GameEventBus& event_bus, std::vector<std::unique_ptr<GameObject> > &game_objects);
};