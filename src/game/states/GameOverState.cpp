//
// Created by marcel on 9/29/25.
//

#include "GameOverState.h"

#include "RunState.h"

#include "engine/modules/assets/AssetModule.h"
#include "engine/modules/input/InputModule.h"
#include "engine/modules/graphics/RenderModule.h"


namespace Game::State {
    GameOverState::GameOverState(Asteroids& game) : m_game(game) {}


    GameOverState::~GameOverState() = default;


    std::unique_ptr<Engine::Core::IGameState> GameOverState::handleInput(Engine::Input::InputManager& input_manager) {

        if (input_manager.wasKeyPressed(Engine::Input::Key::ESCAPE)) return std::make_unique<RunState>(m_game);

        return nullptr;
    }

    std::unique_ptr<Engine::Core::IGameState> GameOverState::update(float deltatime, Engine::Application& engine) {


        // std::unique_ptr<Engine::Core::GameObject> Pause = std::make_unique<Engine::Core::GameObject>();
        //
        // Engine::Assets::Font* font = engine.getAssetManager().getFont("pixel", 72);
        // Pause->addComponent<Engine::Graphics::UITextComponent>("GAME OVER", font, Engine::Core::Color{255, 255, 255}, 210, 340);
        //
        // m_pause.push_back(std::move(Pause));


        return nullptr;
    }

    void GameOverState::render(Engine::Application& engine) {

        // Engine::Graphics::RenderSystem& renderer = engine.getRenderer();
        //
        //
        // renderer.renderWorld(m_game.getGameObjects(), engine.getAssetManager());
        // renderer.renderUI(m_game.getUIObjects(), engine.getAssetManager());
        //
        // renderer.drawRect({0, 0, 800, 800}, Engine::Core::Color{0, 0, 0, 192});
        // renderer.renderUI(m_pause);
    }
}  // namespace Game::State