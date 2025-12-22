//
// Created by marcel on 9/29/25.
//

#pragma once
#include <memory>

#include "engine/core/CoreModule.h"
#include "game/core/CoreModule.h"


namespace Game::State {
    class GameOverState : public Engine::Core::IGameState {

    public:
        GameOverState(Asteroids& game);
        ~GameOverState()                            override;

        std::unique_ptr<Engine::Core::IGameState>                         handleInput(Engine::Input::InputManager& input_manager)    override;
        std::unique_ptr<Engine::Core::IGameState>                         update(float deltatime, Engine::Application& engine)     override;
        void                                                render(Engine::Graphics::RenderSystem& renderer)              override;

    private:
        Asteroids&                                          m_game;
        std::vector<std::unique_ptr<Engine::Core::GameObject>>            m_pause;
    };
}