//
// Created by marcel on 9/28/25.
//

#pragma once

#include "engine/core/CoreModule.h"

namespace Engine::Core { class Context; }

namespace Game {
    // Empty playground for trying out engine features without touching the real game
    class Sandbox : public Engine::Core::IGame {

    public:
        Sandbox();
        ~Sandbox() override;

        bool init(Engine::Core::Context& ctx) override;
        void handleInput(Engine::Core::Context& ctx) override;
        bool update(float deltatime, Engine::Core::Context& ctx) override;
        void render(Engine::Core::Context& ctx) override;
    };
}  // namespace Game
