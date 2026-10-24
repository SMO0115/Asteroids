//
// Created by smo0115 on 10/24/26.
//

#pragma once
#include <memory>

#include "engine/core/CoreModule.h"
#include "game/core/CoreModule.h"

namespace Game::State {
// The title screen: a few asteroids drift behind the title and the high score. ENTER starts a game.
class MenuState : public Engine::Core::IGameState {
public:
    MenuState(Asteroids& game);
    ~MenuState() override;

    void                                      onEnter(Engine::Core::Context& ctx) override;

    std::unique_ptr<Engine::Core::IGameState> handleInput(Engine::Core::Context& ctx) override;
    std::unique_ptr<Engine::Core::IGameState> update(float deltatime, Engine::Core::Context& ctx) override;
    void                                      render(Engine::Core::Context& ctx) override;

private:
    Asteroids& m_game;
    float      m_time = 0.0f;  // seconds in this state, to blink "PRESS ENTER"
};
}  // namespace Game::State
