//
// Created by marcel on 9/29/25.
//

#pragma once
#include <memory>


#include "engine/core/CoreModule.h"
#include "game/core/CoreModule.h"

class PauseState : public IGameState {

public:
    PauseState(Asteroids& game);
    ~PauseState() override;

    std::unique_ptr<IGameState> handleInput(InputManager& input_manager) override;
    std::unique_ptr<IGameState> update(float deltatime, Engine& engine) override;
    void render(RenderSystem& renderer) override;
private:
    Asteroids& m_game;
    std::vector<std::unique_ptr<GameObject>>            m_pause;
};