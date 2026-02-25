//
// Created by marcel on 2/10/26.
//


#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>


namespace Engine::Core { class GameObject; }
namespace Engine::Core { class Context; }
namespace Engine::Assets { class AssetManager; }

namespace Game::Player {
    class PlayerSystem {
        public:
        PlayerSystem() = default;
        ~PlayerSystem() = default;

        void update(Engine::Core::Context& ctx);
    };
}