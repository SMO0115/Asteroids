//
// Created by marcel on 9/28/25.
//

#include "glm/glm.hpp"


#include "Asteroids.h"

#include "engine/core/CoreModule.h"

#include "engine/events/EngineEvents.h"
#include "engine/events/EventBus.h"
#include "engine/modules/assets/AssetModule.h"
#include "engine/modules/audio/AudioModule.h"
#include "engine/scripting/ComponentRegistry.h"
#include "engine/scripting/StateRegistry.h"


#include "GameSession.h"
#include "GameTypes.h"
#include "HighScore.h"
#include "SessionSystem.h"

#include "components/Tags.h"

#include "game/events/GameEvents.h"
#include "game/modules/asteroid/AsteroidModule.h"
#include "game/modules/effects/EffectsSystem.h"
#include "game/modules/player/PlayerModule.h"
#include "game/scripting/binding/ComponentBinding.h"
#include "game/states/MenuState.h"
#include "game/ui/Hud.h"


namespace Game {
Asteroids::Asteroids() {}

Asteroids::~Asteroids() {}


bool Asteroids::init(Engine::Core::Context& ctx) {

    // Teach the engine our component, state and pool names BEFORE running the scripts that use them
    Scripting::registerGameComponents(ctx.get<Engine::Scripting::ComponentRegistry>());
    Scripting::registerGameStates(ctx.get<Engine::Scripting::StateRegistry>());
    Scripting::registerGamePools(ctx.get<Engine::Core::Scene>());

    ctx.get<Engine::Scripting::ScriptManager>().runFile("assets/scripts/asset_loader.lua");
    ctx.get<Engine::Scripting::ScriptManager>().runFile("assets/scripts/prefabs.lua");  // only defines Spawn...() functions

    // Registered up front, so publishing them never has to create a queue in the middle of a tick
    auto& event_bus = ctx.get<Engine::Events::EventBus>();
    event_bus.registerEvent<Events::AsteroidDestroyedEvent>();
    event_bus.registerEvent<Events::ShipDestroyedEvent>();
    event_bus.registerEvent<Engine::Events::PlaySoundEvent>();

    ctx.registerService<Game::Core::GameSession>();
    ctx.registerService<Game::Core::HighScore>();
    ctx.registerService<Game::Core::SessionSystem>();
    ctx.registerService<Game::UI::Hud>();
    ctx.registerService<Game::Player::PlayerSystem>();
    ctx.registerService<Game::Asteroid::AsteroidSystem>();
    ctx.registerService<Game::Asteroid::WaveSystem>();
    ctx.registerService<Game::Effects::EffectsSystem>();

    // The title screen first; it calls startNewGame() when you press ENTER
    m_state = std::make_unique<State::MenuState>(*this);
    m_state->onEnter(ctx);

    return true;
}


// Called between ticks' system updates (from init() or a state's handleInput()), never while a
// system loops over the Scene or an event points into it: Scene::clear() deletes every object.
void Asteroids::startNewGame(Engine::Core::Context& ctx) {

    ctx.get<Engine::Audio::SoundSystem>().stopAll();  // a looping sound of a deleted object would never stop
    ctx.get<Engine::Core::Scene>().clear();

    ctx.get<Game::Core::GameSession>().reset();
    ctx.get<Game::Asteroid::WaveSystem>().reset();

    // Lua spawns the ship and the bullet pool; SpawnEntity resolves their asset names right away
    ctx.get<Engine::Scripting::ScriptManager>().call("StartGame");
}


void Asteroids::handleInput(Engine::Core::Context& ctx) { changeState_(m_state->handleInput(ctx), ctx); }


bool Asteroids::update(float deltatime, Engine::Core::Context& ctx) {

    changeState_(m_state->update(deltatime, ctx), ctx);
    return true;
}

void Asteroids::render(Engine::Core::Context& ctx) { m_state->render(ctx); }


// nullptr = stay in the current state
void Asteroids::changeState_(std::unique_ptr<Engine::Core::IGameState> new_state, Engine::Core::Context& ctx) {

    if (new_state == nullptr) return;

    m_state->onExit(ctx);
    m_state = std::move(new_state);
    m_state->onEnter(ctx);
}
}  // namespace Game
