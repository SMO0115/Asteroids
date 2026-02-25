//
// Created by marcel on 9/28/25.
//

#include "glm/glm.hpp"


#include "Asteroids.h"

#include "engine/core/CoreModule.h"

#include "engine/modules/assets/AssetModule.h"


#include "GameTypes.h"

#include "components/Tags.h"

#include "game/events/GameEvents.h"
#include "game/modules/player/PlayerModule.h"
#include "game/states/RunState.h"


namespace Game {
Asteroids::Asteroids() {}

Asteroids::~Asteroids() {}


bool Asteroids::init(Engine::Core::Context& ctx) {

    Engine::Core::Scene& scene = ctx.get<Engine::Core::Scene>();
    // Pool&                entities  = scene.registerPool("Entities");
    // Pool&                ui        = scene.registerPool("UI");
    // Pool&                GameState = scene.registerPool("GameState");

    ctx.get<Engine::Scripting::ScriptManager>().runFile("assets/scripts/asset_loader.lua");
    ctx.get<Engine::Scripting::ScriptManager>().runFile("assets/scripts/ent_creation.lua");

    Engine::Assets::resolveAssets(ctx);

    ctx.registerService<Game::Player::PlayerSystem>();

    m_state = std::make_unique<State::RunState>(*this);
    m_state->onEnter();

    return true;
}


void Asteroids::handleInput(Engine::Input::InputManager& input_manager) {

    std::unique_ptr<Engine::Core::IGameState> new_state = m_state->handleInput(input_manager);

    if (new_state != nullptr) {

        m_state->onExit();
        m_state = std::move(new_state);
        m_state->onEnter();
    }
}


bool Asteroids::update(float deltatime, Engine::Core::Context& ctx) {

    std::unique_ptr<Engine::Core::IGameState> new_state = m_state->update(deltatime, ctx);

    if (new_state != nullptr) {

        m_state->onExit();
        m_state = std::move(new_state);
        m_state->onEnter();
    }

    return true;
}

void Asteroids::render(Engine::Core::Context& ctx) { m_state->render(ctx); }
}  // namespace Game
