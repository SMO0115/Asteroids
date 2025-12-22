//
// Created by marcel on 11/17/25.
//

#pragma once
#include <vector>
#include <memory>
#include "../../events/GameEvents.h"

namespace Engine::Core { class GameObject; }


namespace Game::Emitter {
    class EmitterSystem {

    public:

        EmitterSystem() = default;
        ~EmitterSystem() = default;

        void update(float deltaTime, Events::GameEventBus& event_bus, std::vector<std::unique_ptr<Engine::Core::GameObject> > &game_objects);
    };
}