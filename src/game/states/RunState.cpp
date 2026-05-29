//
// Created by marcel on 9/29/25.
//

#include "RunState.h"
#include "PauseState.h"

#include "engine/core/CoreModule.h"
#include "engine/modules/assets/AssetManager.h"
#include "engine/modules/graphics/AnimationSystem.h"
#include "engine/modules/graphics/RenderSystem.h"
#include "engine/modules/input/InputModule.h"
#include "engine/modules/physics/PhysicsSystem.h"
#include "game/core/screenWrapper.h"

#include "game/modules/player/PlayerModule.h"


namespace Game::State {
RunState::~RunState() = default;


std::unique_ptr<Engine::Core::IGameState> RunState::handleInput(Engine::Core::Context& ctx) {

    Engine::Input::InputManager& input_manager = ctx.get<Engine::Input::InputManager>();

    if (input_manager.wasKeyPressed(Engine::Input::Key::ESCAPE)) return std::make_unique<PauseState>(m_game);

    return nullptr;
}


std::unique_ptr<Engine::Core::IGameState> RunState::update(float deltatime, Engine::Core::Context& ctx) {

    auto game_objects = ctx.get<Engine::Core::Scene>().getAll();  //.getPool(Game::Core::GamePools::ENEMIES);

    Game::Core::screenWrapper(ctx);

    ctx.get<Game::Player::PlayerSystem>().update(ctx);
    ctx.get<Engine::Physics::PhysicsSystem>().update(ctx);
    ctx.get<Engine::Graphics::AnimationSystem>().update(ctx);

    return nullptr;
}

void RunState::render(Engine::Core::Context& ctx) {

    Engine::Graphics::RenderSystem& renderer      = ctx.get<Engine::Graphics::RenderSystem>();
    Engine::Core::Scene&            scene         = ctx.get<Engine::Core::Scene>();
    Engine::Assets::AssetManager&   asset_manager = ctx.get<Engine::Assets::AssetManager>();


    auto& a = scene.getPool(Game::Core::GamePools::PLAYER);
    auto& b = scene.getPool(Game::Core::GamePools::ENEMIES);

    renderer.renderWorld(scene.getPool(Game::Core::GamePools::PLAYER), asset_manager);
    renderer.renderWorld(scene.getPool(Game::Core::GamePools::ENEMIES), asset_manager);
    renderer.renderWorld(scene.getPool(Game::Core::GamePools::BULLETS), asset_manager);
    renderer.renderUI(scene.getPool(Game::Core::GamePools::UI), asset_manager);
}
}  // namespace Game::State
