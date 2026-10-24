//
// Created by marcel on 9/29/25.
//

#include "GameOverState.h"

#include <cmath>

#include "MenuState.h"
#include "RunState.h"

#include "engine/core/Log.h"
#include "engine/modules/graphics/RenderModule.h"
#include "engine/modules/input/InputModule.h"
#include "game/core/GameSession.h"
#include "game/core/HighScore.h"
#include "game/ui/Text.h"


namespace Game::State {
GameOverState::GameOverState(Asteroids& game) : m_game(game) {}


GameOverState::~GameOverState() = default;


void GameOverState::onEnter(Engine::Core::Context& ctx) {

    const int score = ctx.get<Game::Core::GameSession>().score;
    m_new_record    = ctx.get<Game::Core::HighScore>().submit(score);
    LOG_INFO("Game over: {} points{}", score, m_new_record ? " (new high score)" : "");
}


std::unique_ptr<Engine::Core::IGameState> GameOverState::handleInput(Engine::Core::Context& ctx) {

    Engine::Input::InputManager& input_manager = ctx.get<Engine::Input::InputManager>();

    if (input_manager.wasKeyPressed(Engine::Input::Key::ENTER)) {
        m_game.startNewGame(ctx);
        return std::make_unique<RunState>(m_game);
    }

    if (input_manager.wasKeyPressed(Engine::Input::Key::ESCAPE)) return std::make_unique<MenuState>(m_game);

    return nullptr;
}


std::unique_ptr<Engine::Core::IGameState> GameOverState::update(float deltatime, Engine::Core::Context& ctx) {

    m_time += deltatime;
    RunState::updateWorld(ctx, deltatime);  // the asteroids drift on
    return nullptr;
}


void GameOverState::render(Engine::Core::Context& ctx) {

    const int middle = ctx.get<Engine::Graphics::RenderSystem>().getScreenSize().y / 2;

    RunState::renderWorld(ctx);  // with the HUD: it shows the final score

    UI::drawCenteredText(ctx, "GAME OVER", UI::BIG_TEXT, middle - UI::BIG_TEXT);
    if (m_new_record) UI::drawCenteredText(ctx, "NEW HIGH SCORE!", UI::SMALL_TEXT, middle + 10);

    // Blink: visible for the first 0.6 s of every second
    if (std::fmod(m_time, 1.0f) < 0.6f) UI::drawCenteredText(ctx, "PRESS ENTER", UI::SMALL_TEXT, middle + 80);
}
}  // namespace Game::State
