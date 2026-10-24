//
// Created by smo0115 on 10/16/26.
//

#pragma once

namespace Engine::Core      { class Scene; }
namespace Engine::Scripting { class ComponentRegistry; class StateRegistry; }

namespace Game::Scripting {

    // Everything Asteroids teaches the engine before the Lua scripts run (called from Asteroids::init)
    void registerGameComponents(Engine::Scripting::ComponentRegistry& components);  // "Ship", "Asteroid", ...
    void registerGameStates(Engine::Scripting::StateRegistry& states);              // "IDLE", "ASTEROID", ...
    void registerGamePools(Engine::Core::Scene& scene);                             // "Player", "Enemies", ...
}  // namespace Game::Scripting
