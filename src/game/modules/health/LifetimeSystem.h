//
// Created by marcel on 12/6/25.
//

#pragma once


#include <vector>
#include <memory>
#include "../../events/GameEvents.h"


namespace Engine::Core { class GameObject; }


namespace Game::Health {
    class LifetimeSystem {
    public:
        LifetimeSystem() = default;
        ~LifetimeSystem() = default;

        void update(float deltaTime, Events::GameEventBus& event_bus, std::vector< std::unique_ptr<Engine::Core::GameObject> > &game_objects);
    };
}