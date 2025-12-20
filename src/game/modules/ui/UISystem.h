//
// Created by marcel on 12/13/25.
//

#pragma once

#include <vector>
#include <memory>
#include "../../../engine/events/EngineEvents.h"

class GameStateComponent;

class UISystem {
    public:
    UISystem() = default;
    ~UISystem() = default;

    void update(
        float deltaTime,
        std::vector<std::unique_ptr<GameObject> > &ui_objects,
        const GameStateComponent &game_state
    );
};
