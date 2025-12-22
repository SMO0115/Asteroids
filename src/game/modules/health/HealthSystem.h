//
// Created by marcel on 11/23/25.
//

#pragma once

#include <vector>
#include <memory>
#include "../../../engine/events/EngineEvents.h"

namespace Engine::Core { class GameObject; }



namespace Game::Health {
    class HealthSystem {
    public:
        HealthSystem() = default;
        ~HealthSystem() = default;

        void update(float deltaTime, Engine::Events::EngineEventBus& event_bus, std::vector<std::unique_ptr<Engine::Core::GameObject> > &game_objects);
    };
}