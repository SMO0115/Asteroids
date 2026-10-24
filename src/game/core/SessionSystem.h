//
// Created by smo0115 on 10/24/26.
//

#pragma once

namespace Engine::Core { class Context; }

namespace Game::Core {

    // The bookkeeping of the rules: reads this tick's game events and updates the GameSession
    // (points per asteroid and saucer, a life lost per ship, an extra life every 10,000 points, with its sound)
    class SessionSystem {
    public:
        SessionSystem()  = default;
        ~SessionSystem() = default;

        void update(Engine::Core::Context& ctx);
    };
}  // namespace Game::Core
