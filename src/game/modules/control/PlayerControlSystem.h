//
// Created by marcel on 11/8/25.
//


#pragma once

#include <vector>
#include <memory>
#include "../../../engine/events/EngineEvents.h"
#include "../../events/GameEvents.h"

class GameObject;
class InputManager;

class PlayerControlSystem {

public:
    PlayerControlSystem();
    ~PlayerControlSystem();
    void update(float deltaTime, InputManager& input_manager, GameEventBus& game_event_bus, EngineEventBus& engine_event_bus, const std::vector<std::unique_ptr<GameObject> > &game_objects);
};

