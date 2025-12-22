//
// Created by marcel on 11/8/25.
//

#pragma once

#include <vector>
#include <memory>
#include "../../events/GameEvents.h"
#include "../../../engine/events/EngineEvents.h"

namespace Engine::Core { class GameObject; }
namespace Engine::Input { class InputManager; }

namespace Game::Control {
    class AISystem {
    public:
        AISystem() = default;
        ~AISystem() = default;

        void update(float deltaTime, Engine::Events::EngineEventBus& engine_event_bus, Events::GameEventBus& game_event_bus, std::vector<std::unique_ptr<Engine::Core::GameObject> > &game_objects);

    private:
        float m_shootTimer = 0.0f;
        float m_shootInterval = 1.5f;
    };
}