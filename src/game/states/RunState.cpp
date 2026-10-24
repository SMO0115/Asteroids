//
// Created by marcel on 9/29/25.
//

#include "RunState.h"
#include "GameOverState.h"
#include "PauseState.h"

#include "engine/core/CoreModule.h"
#include "engine/core/modules/LifeModule.h"
#include "engine/modules/assets/AssetManager.h"
#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/graphics/AnimationSystem.h"
#include "engine/modules/graphics/RenderSystem.h"
#include "engine/modules/input/InputModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "game/core/GameSession.h"
#include "game/core/SessionSystem.h"
#include "game/core/screenWrapper.h"
#include "game/ui/Hud.h"

#include "game/modules/asteroid/AsteroidModule.h"
#include "game/modules/combat/CombatSystem.h"
#include "game/modules/effects/EffectsSystem.h"
#include "game/modules/player/PlayerModule.h"
#include "game/modules/saucer/SaucerModule.h"


namespace Game::State {

constexpr float GAME_OVER_DELAY = 2.0f;  // seconds to watch the last ship's wreck before "GAME OVER"


RunState::~RunState() = default;


std::unique_ptr<Engine::Core::IGameState> RunState::handleInput(Engine::Core::Context& ctx) {

    Engine::Input::InputManager& input_manager = ctx.get<Engine::Input::InputManager>();

    if (input_manager.wasKeyPressed(Engine::Input::Key::ESCAPE)) return std::make_unique<PauseState>(m_game);

    return nullptr;
}


std::unique_ptr<Engine::Core::IGameState> RunState::update(float deltatime, Engine::Core::Context& ctx) {

    // Detect, then respond: the CombatSystem reacts to this tick's CollisionEvents
    ctx.get<Engine::Physics::CollisionSystem>().update(ctx);
    ctx.get<Game::Combat::CombatSystem>().update(ctx);

    // After the CombatSystem (it reacts to this tick's ship hit), but BEFORE the session and the
    // effects: a failed hyperspace jump destroys the ship too, and that must cost a life and make a
    // bang in this same tick. (Events are cleared at the end of the tick, so a system that runs
    // before the publisher never sees the event.)
    ctx.get<Game::Player::PlayerSystem>().update(ctx, deltatime);
    ctx.get<Game::Saucer::SaucerSystem>().update(ctx, deltatime);

    ctx.get<Game::Core::SessionSystem>().update(ctx);              // points and lives for this tick's hits
    ctx.get<Game::Effects::EffectsSystem>().update(ctx);           // bangs and debris for the same hits
    ctx.get<Game::Asteroid::WaveSystem>().update(ctx, deltatime);  // after the hits: was that the last asteroid?

    updateWorld(ctx, deltatime);  // moves everything, including what the keys did above

    ctx.get<Engine::Audio::SoundSystem>().update(ctx);  // last: plays what everyone above asked for in this tick

    if (ctx.get<Game::Core::GameSession>().lives <= 0) {
        m_game_over_timer += deltatime;
        if (m_game_over_timer >= GAME_OVER_DELAY) return std::make_unique<GameOverState>(m_game);
    }

    return nullptr;
}


void RunState::updateWorld(Engine::Core::Context& ctx, float deltatime) {

    ctx.get<Engine::Physics::PhysicsSystem>().update(ctx, deltatime);
    Game::Core::screenWrapper(ctx);  // right after moving: whatever flew off one side comes back on the other
    ctx.get<Engine::Core::LifeSystem>().update(ctx, deltatime);
    ctx.get<Engine::Graphics::AnimationSystem>().update(ctx, deltatime);
}


void RunState::render(Engine::Core::Context& ctx) { renderWorld(ctx); }


void RunState::renderScene(Engine::Core::Context& ctx) {

    Engine::Graphics::RenderSystem& renderer      = ctx.get<Engine::Graphics::RenderSystem>();
    Engine::Core::Scene&            scene         = ctx.get<Engine::Core::Scene>();
    Engine::Assets::AssetManager&   asset_manager = ctx.get<Engine::Assets::AssetManager>();

    renderer.renderWorld(scene.getPool(Game::Core::GamePools::PLAYER), asset_manager);
    renderer.renderWorld(scene.getPool(Game::Core::GamePools::ENEMIES), asset_manager);
    renderer.renderWorld(scene.getPool(Game::Core::GamePools::BULLETS), asset_manager);
    renderer.renderWorld(scene.getPool(Game::Core::GamePools::SAUCER_BULLETS), asset_manager);
    renderer.renderWorld(scene.getPool(Game::Core::GamePools::EFFECTS), asset_manager);
    renderer.renderUI(scene.getPool(Game::Core::GamePools::UI), asset_manager);
}


void RunState::renderWorld(Engine::Core::Context& ctx) {

    renderScene(ctx);
    ctx.get<Game::UI::Hud>().render(ctx);
}
}  // namespace Game::State
