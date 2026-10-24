//
// Created by marcel on 9/29/25.
//

#include "game/states/PauseState.h"
#include "RunState.h"

#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/graphics/RenderModule.h"
#include "engine/modules/input/InputModule.h"
#include "game/ui/Text.h"


namespace Game::State {
PauseState::PauseState(Asteroids& game) : m_game(game) {}


PauseState::~PauseState() = default;


// The SoundSystem doesn't run while paused, so a looping sound (the thrust) would go on forever
void PauseState::onEnter(Engine::Core::Context& ctx) { ctx.get<Engine::Audio::SoundSystem>().pauseAll(); }

void PauseState::onExit(Engine::Core::Context& ctx) { ctx.get<Engine::Audio::SoundSystem>().resumeAll(); }


std::unique_ptr<Engine::Core::IGameState> PauseState::handleInput(Engine::Core::Context& ctx) {

    Engine::Input::InputManager& input_manager = ctx.get<Engine::Input::InputManager>();

    if (input_manager.wasKeyPressed(Engine::Input::Key::ESCAPE)) return std::make_unique<RunState>(m_game);

    return nullptr;
}

// Nothing updates while paused: that is what freezes the game
std::unique_ptr<Engine::Core::IGameState> PauseState::update(float /*deltatime*/, Engine::Core::Context& /*ctx*/) { return nullptr; }

void PauseState::render(Engine::Core::Context& ctx) {

    Engine::Graphics::RenderSystem& renderer = ctx.get<Engine::Graphics::RenderSystem>();
    const glm::ivec2                screen   = renderer.getScreenSize();

    // The frozen game, darkened, with the label on top (render() used to be empty: a black screen)
    RunState::renderWorld(ctx);
    renderer.drawRect({0, 0, screen.x, screen.y}, Engine::Core::Color{0, 0, 0, 160});

    UI::drawCenteredText(ctx, "PAUSED", UI::BIG_TEXT, screen.y / 2 - UI::BIG_TEXT / 2);
}
}  // namespace Game::State
