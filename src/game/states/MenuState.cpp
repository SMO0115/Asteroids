//
// Created by smo0115 on 10/24/26.
//

#include "MenuState.h"

#include <cmath>
#include <string>

#include "RunState.h"

#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/graphics/RenderModule.h"
#include "engine/modules/input/InputModule.h"
#include "game/core/HighScore.h"
#include "game/modules/asteroid/WaveSystem.h"
#include "game/ui/Text.h"


namespace Game::State {

constexpr int BACKGROUND_ASTEROIDS = 4;


MenuState::MenuState(Asteroids& game) : m_game(game) {}


MenuState::~MenuState() = default;


// A fresh sky with only a few asteroids. onEnter runs between system updates (Asteroids::changeState_),
// so clearing the Scene here is safe.
void MenuState::onEnter(Engine::Core::Context& ctx) {

    ctx.get<Engine::Audio::SoundSystem>().stopAll();
    ctx.get<Engine::Core::Scene>().clear();

    const glm::vec2 center = glm::vec2(ctx.get<Engine::Graphics::RenderSystem>().getScreenSize()) / 2.0f;
    Asteroid::WaveSystem::spawnAsteroids(ctx, BACKGROUND_ASTEROIDS, center);
}


std::unique_ptr<Engine::Core::IGameState> MenuState::handleInput(Engine::Core::Context& ctx) {

    Engine::Input::InputManager& input_manager = ctx.get<Engine::Input::InputManager>();

    if (input_manager.wasKeyPressed(Engine::Input::Key::ENTER)) {
        m_game.startNewGame(ctx);
        return std::make_unique<RunState>(m_game);
    }

    return nullptr;
}


std::unique_ptr<Engine::Core::IGameState> MenuState::update(float deltatime, Engine::Core::Context& ctx) {

    m_time += deltatime;
    RunState::updateWorld(ctx, deltatime);
    return nullptr;
}


void MenuState::render(Engine::Core::Context& ctx) {

    const int height = ctx.get<Engine::Graphics::RenderSystem>().getScreenSize().y;

    RunState::renderScene(ctx);  // no HUD: there is no game yet

    UI::drawCenteredText(ctx, "ASTEROIDS", UI::BIG_TEXT, height / 4);
    UI::drawCenteredText(ctx, "HIGH SCORE " + std::to_string(ctx.get<Game::Core::HighScore>().best()), UI::SMALL_TEXT, height / 4 + 100);

    // Blink: visible for the first 0.6 s of every second
    if (std::fmod(m_time, 1.0f) < 0.6f) UI::drawCenteredText(ctx, "PRESS ENTER", UI::SMALL_TEXT, height / 2 + 60);

    UI::drawCenteredText(ctx, "ARROWS: FLY     SPACE: FIRE", UI::SMALL_TEXT, height - 130);
    UI::drawCenteredText(ctx, "DOWN: HYPERSPACE     ESC: PAUSE", UI::SMALL_TEXT, height - 85);
}
}  // namespace Game::State
