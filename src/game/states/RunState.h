//
// Created by marcel on 9/29/25.
//

#pragma once
#include <memory>

#include "../../engine/core/interfaces/IGameState.h"

namespace Game {
class Asteroids;
}

namespace Game::State {
class RunState : public Engine::Core::IGameState {
public:
    RunState(Asteroids& game) : m_game(game) {}
    ~RunState() override;

    std::unique_ptr<Engine::Core::IGameState> handleInput(Engine::Input::InputManager& input_manager) override;
    std::unique_ptr<Engine::Core::IGameState> update(float deltatime, Engine::Application& engine) override;
    void                                      render(Engine::Application& engine) override;

private:
    Asteroids& m_game;
};
}  // namespace Game::State