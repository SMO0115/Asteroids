//
// Created by marcel on 9/29/25.
//

#include "RunState.h"
#include "GameOverState.h"
#include "LevelCompletedState.h"
#include "PauseState.h"

#include "engine/modules/input/InputModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/rendering/RenderModule.h"
#include "engine/modules/audio/AudioModule.h"

#include "game/modules/control/ControlModul.h"
#include "game/modules/emitter/EmiterModule.h"
#include "game/modules/health/HealthModule.h"
#include "game/modules/ui/UIModule.h"


namespace Game::State {
    RunState::~RunState() = default;


    std::unique_ptr<Engine::Core::IGameState> RunState::handleInput(Engine::Input::InputManager &input_manager) {

        if ( input_manager.wasKeyPressed(Engine::Input::Key::ESCAPE) ) return std::make_unique<PauseState>(m_game);

        return nullptr;
    }


    std::unique_ptr<Engine::Core::IGameState> RunState::update(float deltatime, Engine::Application& engine) {

        auto& game_state = m_game.getGameSession()->getComponent<UI::GameStateComponent>();
        std::vector<std::unique_ptr<Engine::Core::GameObject>>& game_objects = m_game.getGameObjects();
        std::vector<std::unique_ptr<Engine::Core::GameObject>>& ui_objects    = m_game.getUIObjects();


        m_game.getPlayerControlSystem().update(deltatime, engine.getInputManager(), m_game.getEventBus(), engine.getEventBus(), game_objects);
        m_game.getAISystem().update(deltatime, engine.getEventBus(), m_game.getEventBus(), game_objects);


        engine.getPhysicsSystem().update(deltatime, game_objects);


        engine.getCollisionSystem().update(engine.getEventBus(), game_objects);


        m_game.getHealthSystem().update(deltatime, engine.getEventBus(), game_objects);
        m_game.getLifetimeSystem().update(deltatime, m_game.getEventBus(), game_objects);
        m_game.getScoreSystem().update(deltatime, m_game.getEventBus(), game_state);


        m_game.getEmitterSystem().update(deltatime, m_game.getEventBus(), game_objects);
        engine.getSoundSystem().update(engine.getAssetManager(), engine.getEventBus(), game_objects);
        engine.getAnimationSystem().update(deltatime, game_objects);
        m_game.getUISystem().update(deltatime, ui_objects, game_state);


        if (game_state.lives        == 0) return std::make_unique<GameOverState>(m_game);
        if (game_state.num_invaders == 0) return std::make_unique<LevelCompletedState>(m_game);

        return nullptr;
    }

    void RunState::render(Engine::Graphics::RenderSystem &renderer) {

        renderer.renderWorld(m_game.getGameObjects());
        renderer.renderUI(m_game.getUIObjects());
    }
}









