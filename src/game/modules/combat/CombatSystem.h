//
// Created by smo0115 on 10/24/26.
//

#pragma once

namespace Engine::Core { class Context; class GameObject; }

namespace Game::Combat {

    // Collision RESPONSE (the CollisionSystem only detects). In Asteroids every collision destroys
    // BOTH objects: a bullet is used up, a ship or a saucer explodes, an asteroid splits. So the rule
    // is short: for each colliding pair, kill both; kill_() knows what dying means for each kind.
    // Publishes AsteroidDestroyedEvent, SaucerDestroyedEvent and ShipDestroyedEvent for everyone
    // else who reacts (score, respawn, effects, ...).
    //
    // (This was the AsteroidSystem, which only knew "something hit an asteroid". With saucers, ships
    // and asteroids are also hit by saucer shots, and saucers by everything.)
    class CombatSystem {
    public:
        CombatSystem()  = default;
        ~CombatSystem() = default;

        void update(Engine::Core::Context& ctx);

    private:
        static void kill_(Engine::Core::Context& ctx, Engine::Core::GameObject& victim, const Engine::Core::GameObject& killer);
        static void split_(Engine::Core::Context& ctx, Engine::Core::GameObject& asteroid);
    };
}  // namespace Game::Combat
