//
// Created by smo0115 on 10/16/26.
//

#pragma once

namespace Engine::Core { class Context; class GameObject; }

namespace Game::Asteroid {

    // Collision RESPONSE for asteroids (CollisionSystem only detects): whatever hit the asteroid
    // is switched off, and the asteroid splits into two smaller ones. Publishes AsteroidDestroyedEvent
    // and ShipDestroyedEvent for everyone else who reacts (score, respawn, ...).
    class AsteroidSystem {
    public:
        AsteroidSystem()  = default;
        ~AsteroidSystem() = default;

        void update(Engine::Core::Context& ctx);

    private:
        void split_(Engine::Core::Context& ctx, Engine::Core::GameObject& asteroid);
    };
}  // namespace Game::Asteroid
