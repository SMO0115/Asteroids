//
// Created by marcel on 11/9/25.
//

#pragma once
#include <vector>
#include <memory>

namespace Engine::Core { class GameObject; }

namespace Engine::Physics {
    class PhysicsSystem {

    public:
                                PhysicsSystem()  = default;
                                ~PhysicsSystem() = default;

        void                    update(float deltatime,  std::vector<std::unique_ptr<Core::GameObject> > &game_objects);
    };
}