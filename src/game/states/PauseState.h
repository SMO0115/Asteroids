//
// Created by marcel on 9/29/25.
//

#pragma once
#include <memory>

#include "engine/core/CoreModule.h"
#include "game/core/CoreModule.h"

namespace Game::State {
class PauseState : public Engine::Core::IGameState {
public:
    PauseState(Asteroids& game);
    ~PauseState() override;

    void                                      onEnter(Engine::Core::Context& ctx) override;
    void                                      onExit(Engine::Core::Context& ctx) override;

    std::unique_ptr<Engine::Core::IGameState> handleInput(Engine::Core::Context& ctx) override;
    std::unique_ptr<Engine::Core::IGameState> update(float deltatime, Engine::Core::Context& ctx) override;
    void                                      render(Engine::Core::Context& ctx) override;

private:
    Asteroids& m_game;
};
}  // namespace Game::State
