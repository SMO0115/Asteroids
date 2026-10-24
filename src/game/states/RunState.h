//
// Created by marcel on 9/29/25.
//

#pragma once
#include <memory>

#include "../../engine/core/interfaces/IGameState.h"

namespace Engine::Core { class Context; class IGameState; }
namespace Game { class Asteroids; }

namespace Game::State {
class RunState : public Engine::Core::IGameState {
public:
    RunState(Asteroids& game) : m_game(game) {}
    ~RunState() override;

    std::unique_ptr<Engine::Core::IGameState> handleInput(Engine::Core::Context& ctx) override;
    std::unique_ptr<Engine::Core::IGameState> update(float deltatime, Engine::Core::Context& ctx) override;
    void                                      render(Engine::Core::Context& ctx) override;

    // The parts other states share. The menu and the game-over screen let the asteroids drift on
    // in the background, without the ship, collisions or rules:
    static void                               updateWorld(Engine::Core::Context& ctx, float deltatime);  // move, wrap, age, animate
    static void                               renderScene(Engine::Core::Context& ctx);                   // the objects only
    static void                               renderWorld(Engine::Core::Context& ctx);                   // the objects + the HUD

private:
    Asteroids& m_game;
    float      m_game_over_timer = 0.0f;  // seconds since the last life was lost
};
}  // namespace Game::State
