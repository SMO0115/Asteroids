//
// Created by marcel on 9/28/25.
//

#pragma once

#include "engine/core/CoreModule.h"

#include <memory>

namespace Engine::Core      { class IGameState;             }
namespace Engine::Core      { class IGame;                  }


namespace Game {
class Asteroids : public Engine::Core::IGame {
public:
    Asteroids();
    ~Asteroids() override;

    bool init(Engine::Core::Context& ctx) override;
    void handleInput(Engine::Core::Context& ctx) override;
    bool update(float deltatime, Engine::Core::Context& ctx) override;
    void render(Engine::Core::Context& ctx) override;

    // Empties the world and starts over: score, lives and waves back to the start
    void startNewGame(Engine::Core::Context& ctx);

private:
    void changeState_(std::unique_ptr<Engine::Core::IGameState> new_state, Engine::Core::Context& ctx);

    std::unique_ptr<Engine::Core::IGameState>              m_state;
};
}  // namespace Game
