//
// Created by marcel on 2/10/26.
//


#pragma once


namespace Engine::Core { class Context; class GameObject; }

namespace Game::Player {
    struct ShipComponent;

    // The ship: flying it (keys), and bringing it back after a hit
    class PlayerSystem {
        public:
        PlayerSystem() = default;
        ~PlayerSystem() = default;

        void update(Engine::Core::Context& ctx, float dt);

        // Is there a ship in the game right now (flying, or in hyperspace), or is it destroyed?
        // The heartbeat and the saucers only run while it is.
        static bool shipInPlay(Engine::Core::Context& ctx);

    private:
        void fly_(Engine::Core::Context& ctx, Engine::Core::GameObject& player, ShipComponent& ship, float dt);
        void returnFromHyperspace_(Engine::Core::Context& ctx, Engine::Core::GameObject& player, ShipComponent& ship, float dt);
        void waitForRespawn_(Engine::Core::Context& ctx, Engine::Core::GameObject& player, ShipComponent& ship, float dt);
    };
}
