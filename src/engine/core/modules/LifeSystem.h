//
// Created by marcel on 11/9/25.
//

#pragma once

namespace Engine::Core {
class Context;
}

namespace Engine::Core {

    class LifeSystem {
    public:
        LifeSystem()  = default;
        ~LifeSystem() = default;

        void update(Engine::Core::Context& ctx);
    };
}  // namespace Engine::Physics