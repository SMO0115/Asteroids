//
// Created by marcel on 11/8/25.
//

#pragma once

#include <memory>
#include <vector>
#include "engine/events/EngineEvents.h"
#include "game/events/GameEvents.h"

namespace Engine::Core {
class GameObject;
}
namespace Engine::Input {
class InputManager;
}

namespace Game::Control {
class PlayerControlSystem {
public:
    PlayerControlSystem();
    ~PlayerControlSystem();
    void update(float deltaTime, Engine::Input::InputManager& input_manager, Events::GameEventBus& game_event_bus,
                Engine::Events::EngineEventBus& engine_event_bus, const std::vector<std::unique_ptr<Engine::Core::GameObject> >& game_objects);
};
}  // namespace Game::Control
