//
// Created by marcel on 12/13/25.
//

#pragma once

#include <memory>
#include <vector>
#include "engine/events/EngineEvents.h"

namespace Game::UI {

class GameStateComponent;

class UISystem {
public:
    UISystem()  = default;
    ~UISystem() = default;

    void update(float deltaTime, std::vector<std::unique_ptr<Engine::Core::GameObject> >& ui_objects, const GameStateComponent& game_state);
};
}  // namespace Game::UI