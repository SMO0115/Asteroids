//
// Created by smo0115 on 10/16/26.
//

#pragma once

namespace Engine::Scripting {

    class ComponentRegistry;
    class StateRegistry;

    // Makes the engine's components (Transform, Physics, Collider, Animation, ...) available to Lua.
    // Called once by the Application; the game registers its own components in IGame::init().
    //
    // The factories turn names into numbers with 'states', so the game has to register the names
    // of these enum types: "AnimationState", "SoundState" and "CollisionLayer".
    void registerEngineComponents(ComponentRegistry& components, const StateRegistry& states);
}
