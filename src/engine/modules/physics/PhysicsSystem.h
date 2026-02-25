//
// Created by marcel on 11/9/25.
//

#pragma once
#include <memory>
#include <vector>

namespace Engine::Core {
class Context;
}

namespace Engine::Physics {
class PhysicsSystem {
public:
    PhysicsSystem()  = default;
    ~PhysicsSystem() = default;

    void update(Engine::Core::Context& ctx);
};
}  // namespace Engine::Physics