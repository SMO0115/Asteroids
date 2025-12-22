//
// Created by marcel on 9/29/25.
//

#pragma once

#include <memory>

#include "engine/core/CoreModule.h"
#include "game/core/CoreModule.h"


namespace Game::State {
    class LevelCompletedState : public Engine::Core::IGameState {

    public:
        LevelCompletedState(Asteroids& game);
        ~LevelCompletedState() override;

        std::unique_ptr<Engine::Core::IGameState> handleInput(Engine::Input::InputManager& input_manager) override;
        std::unique_ptr<Engine::Core::IGameState> update(float deltatime, Engine::Application& engine) override;
        void render(Engine::Graphics::RenderSystem& renderer) override;
    private:
        Asteroids& m_game;
        std::vector<std::unique_ptr<Engine::Core::GameObject>>    m_text;
    };
}