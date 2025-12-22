//
// Created by marcel on 9/29/25.
//

#include "../../game/states/LevelCompletedState.h"

#include "RunState.h"

#include "engine/modules/input/InputModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/rendering/RenderModule.h"
#include "engine/modules/assets/AssetModule.h"




namespace Game::State {
    LevelCompletedState::LevelCompletedState(Asteroids& game) : m_game(game) {


    }


    LevelCompletedState::~LevelCompletedState() = default;


    std::unique_ptr<Engine::Core::IGameState> LevelCompletedState::handleInput(Engine::Input::InputManager &input_manager) {

        if ( input_manager.wasKeyPressed(Engine::Input::Key::ESCAPE) ) return std::make_unique<RunState>(m_game);

        return nullptr;
    }

    std::unique_ptr<Engine::Core::IGameState> LevelCompletedState::update(float deltatime, Engine::Application& engine) {



        std::unique_ptr<Engine::Core::GameObject> Pause = std::make_unique<Engine::Core::GameObject>();

        Engine::Assets::Font* font = engine.getAssetManager().getFont("pixel",72);
        Pause->addComponent<Engine::Graphics::UITextComponent>(
            "!!! VICTORY !!!",
            font,
            Engine::Core::Color{255, 255, 255}, 190, 340
            );
        m_text.push_back(std::move(Pause));


        return nullptr;
    }

    void LevelCompletedState::render(Engine::Graphics::RenderSystem &renderer) {

        renderer.renderWorld(m_game.getGameObjects());
        renderer.renderUI(m_game.getUIObjects());

        renderer.drawRect({0, 0, 800, 800}, Engine::Core::Color{0, 0, 0, 192});
        renderer.renderUI(m_text);
    }
}