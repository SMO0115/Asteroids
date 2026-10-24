//
// Created by marcel on 11/9/25.
//

#pragma once

namespace Engine::Core {
class Context;
}

namespace Engine::Core {

    // Counts down LifeComponent::life_time and switches the object off (or destroys it) when it runs out
    class LifeSystem {
    public:
        LifeSystem()  = default;
        ~LifeSystem() = default;

        void update(Engine::Core::Context& ctx, float dt);
    };
}  // namespace Engine::Core