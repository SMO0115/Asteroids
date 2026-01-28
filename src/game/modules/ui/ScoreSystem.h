//
// Created by marcel on 12/16/25.
//

#pragma once

#include "game/events/GameEvents.h"


namespace Engine::Assets { class AssetManager; }

namespace Game::UI {
    class GameStateComponent;

    class ScoreSystem {
    public:
        ScoreSystem()  = default;
        ~ScoreSystem() = default;

        void update(float deltaTime, Events::GameEventBus& game_event_bus, GameStateComponent& game_state, Engine::Assets::AssetManager& assets_manager);
    };
}  // namespace Game::UI