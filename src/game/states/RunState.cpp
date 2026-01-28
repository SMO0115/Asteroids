//
// Created by marcel on 9/29/25.
//

#include "RunState.h"
#include "GameOverState.h"
#include "LevelCompletedState.h"
#include "PauseState.h"

#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/input/InputModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/graphics/RenderModule.h"

#include "game/modules/control/ControlModul.h"
#include "game/modules/emitter/EmiterModule.h"
#include "game/modules/health/HealthModule.h"
#include "game/modules/ui/UIModule.h"


namespace Game::State {
    RunState::~RunState() = default;


    std::unique_ptr<Engine::Core::IGameState> RunState::handleInput(Engine::Input::InputManager& input_manager) {

        if (input_manager.wasKeyPressed(Engine::Input::Key::ESCAPE)) return std::make_unique<PauseState>(m_game);

        return nullptr;
    }


    std::unique_ptr<Engine::Core::IGameState> RunState::update(float deltatime, Engine::Application& engine) {

        auto& game_state   = m_game.getGameSession()->getComponent<UI::GameStateComponent>();
        Pool& game_objects = engine.getScene().getPool("Entities"); // m_game.getGameObjects();
        Pool& ui_objects   = m_game.getUIObjects();


        m_game.getPlayerControlSystem().update(deltatime, engine.getInputManager(), m_game.getEventBus(), engine.getEventBus(), game_objects);
        m_game.getAISystem().update(deltatime, engine.getEventBus(), m_game.getEventBus(), game_objects);


        engine.getPhysicsSystem().update(deltatime, game_objects);


        engine.getCollisionSystem().update(engine.getEventBus(), game_objects);


        m_game.getHealthSystem().update(deltatime, engine.getEventBus(), game_objects, engine.getAssetManager());
        m_game.getLifetimeSystem().update(deltatime, m_game.getEventBus(), game_objects);
        m_game.getScoreSystem().update(deltatime, m_game.getEventBus(), game_state, engine.getAssetManager());


        m_game.getEmitterSystem().update(deltatime, m_game.getEventBus(), game_objects);
        engine.getSoundSystem().update(engine.getAssetManager(), engine.getEventBus(), game_objects);
        engine.getAnimationSystem().update(deltatime, game_objects, engine.getAssetManager());
        m_game.getUISystem().update(deltatime, ui_objects, game_state);


        if (game_state.lives == 0) return std::make_unique<GameOverState>(m_game);
        if (game_state.num_invaders == 0) return std::make_unique<LevelCompletedState>(m_game);

        return nullptr;
    }

    void RunState::render(Engine::Application& engine) {

        Engine::Graphics::RenderSystem& renderer = engine.getRenderer();
        Engine::Core::Scene& scene = engine.getScene();


        renderer.renderWorld(scene.getPool("Entities"), engine.getAssetManager());
        renderer.renderUI(scene.getPool("UI"), engine.getAssetManager());
    }
}  // namespace Game::State
