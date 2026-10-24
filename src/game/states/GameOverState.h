//
// Created by marcel on 9/29/25.
//

#pragma once
#include <memory>

#include "engine/core/CoreModule.h"
#include "game/core/CoreModule.h"

namespace Game::State {
// The last ship is gone: show the score while the asteroids drift on. ENTER plays again,
// ESC goes back to the title screen.
class GameOverState : public Engine::Core::IGameState {
public:
    GameOverState(Asteroids& game);
    ~GameOverState() override;

    void                                      onEnter(Engine::Core::Context& ctx) override;

    std::unique_ptr<Engine::Core::IGameState> handleInput(Engine::Core::Context& ctx) override;
    std::unique_ptr<Engine::Core::IGameState> update(float deltatime, Engine::Core::Context& ctx) override;
    void                                      render(Engine::Core::Context& ctx) override;

private:
    Asteroids& m_game;
    bool       m_new_record = false;
    float      m_time       = 0.0f;  // seconds in this state, to blink "PRESS ENTER"
};
}  // namespace Game::State
