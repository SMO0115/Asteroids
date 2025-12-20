//
// Created by marcel on 12/16/25.
//


#pragma once

#include "../../events/GameEvents.h"

class GameStateComponent;

class ScoreSystem {
public:
    ScoreSystem() = default;
    ~ScoreSystem() = default;

    void update(
            float deltaTime,
            GameEventBus& game_event_bus,
            GameStateComponent &game_state
        );
};
