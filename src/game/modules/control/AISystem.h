//
// Created by marcel on 11/8/25.
//

#pragma once

#include <vector>
#include <memory>
#include "../../events/GameEvents.h"
#include "../../../engine/events/EngineEvents.h"

class GameObject;
class InputManager;


class AISystem {
public:
    AISystem() = default;
    ~AISystem() = default;

    void update(float deltaTime, EngineEventBus& engine_event_bus, GameEventBus& game_event_bus, std::vector<std::unique_ptr<GameObject> > &game_objects);

private:
    float m_shootTimer = 0.0f;
    float m_shootInterval = 1.5f;
};
