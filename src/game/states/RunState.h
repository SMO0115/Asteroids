//
// Created by marcel on 9/29/25.
//

#pragma once
#include <memory>

#include "../../engine/core/IGameState.h"

class Asteroids;

class RunState : public IGameState {

public:
    RunState(Asteroids& game) : m_game(game){}
    ~RunState() override;

    std::unique_ptr<IGameState> handleInput(InputManager& input_manager) override;
    std::unique_ptr<IGameState> update(float deltatime, Engine& engine) override;
    void render(RenderSystem& renderer) override;

private:
    Asteroids& m_game;
};
