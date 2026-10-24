//
// Created by smo0115 on 10/24/26.
//

#pragma once

#include <glm/vec2.hpp>

namespace Engine::Core { class Context; }

namespace Game::Effects {

    // Explosions: for every asteroid, saucer or ship destroyed in this tick, a "bang" and a burst of debris.
    // Purely cosmetic; it only reads events, so the rules work the same without it.
    class EffectsSystem {
    public:
        EffectsSystem()  = default;
        ~EffectsSystem() = default;

        void update(Engine::Core::Context& ctx);

    private:
        static void burst_(Engine::Core::Context& ctx, glm::vec2 position, int pieces, float max_speed, float max_lifetime);
    };
}  // namespace Game::Effects
