//
// Created by marcel on 7/31/25.
//

#include "Application.h"
#include <bits/stdc++.h>
#include <iostream>


#include "engine/modules/assets/AssetModule.h"
#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/input/InputModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/graphics/RenderModule.h"

#include "interfaces/IGame.h"
#include "Log.h"

#include "Scene.h"
#include "engine/core/GameObject.h"


#include "engine/core/TimeManager.h"
#include "../scripting/ScriptManager.h"
#include "engine/events/EventBus.h"

#include "engine/core/components/TransformComponent.h"



namespace Engine {
Application::Application() {

    m_is_running     = 0;
    m_quit_requested = 0;

    m_scene            = std::make_unique<Core::Scene>();

    m_renderer         = std::make_unique<Graphics::RenderSystem>();
    m_animation_system = std::make_unique<Graphics::AnimationSystem>();
    m_sound_system     = std::make_unique<Audio::SoundSystem>();
    m_physics          = std::make_unique<Physics::PhysicsSystem>();
    m_collision_system = std::make_unique<Physics::CollisionSystem>();

    m_time_manager  = std::make_unique<Core::TimeManager>();
    m_asset_manager = std::make_unique<Assets::AssetManager>(m_renderer.get());
    m_input_manager = std::make_unique<Input::InputManager>();

    m_event_bus = std::make_unique<Events::EngineEventBus>();

    m_script_manager = std::make_unique<Scripting::ScriptManager>(*this);

    // m_object_factory->registerComponent<Engine::Core::TransformComponent>("Transform");
    // m_object_factory->registerComponent<Engine::Graphics::SpriteComponent>("Sprite");
    // m_object_factory->registerComponent<Engine::Graphics::AnimationComponent>("Animation");
    // m_object_factory->registerComponent<Engine::Graphics::UITextComponent>("UIText");

    Core::Log::init();
}

Application::~Application() = default;

bool Application::init(const std::string& title, std::size_t width, std::size_t height, glm::uint32_t target_fps) {

    if (!m_renderer->init(title, width, height)) return false;
    if (!m_sound_system->init()) return false;

    return true;
}


void Application::run(Core::IGame& game) {

    if (!game.init(*this)) return;
    m_is_running = true;

    m_time_manager->start();
    constexpr double dt          = 1.0 / 120.0;
    double           accumulator = 0.0;

    while (m_is_running) {

        double frameTime = m_time_manager->tick();
        if (frameTime > 0.25) frameTime = 0.25;

        accumulator += frameTime;

        m_input_manager->processInput();
        game.handleInput(*m_input_manager);
        if (m_input_manager->isKeyHeld(Input::Key::QUIT)) m_is_running = false;


        while (accumulator >= dt) {

            m_is_running = game.update(static_cast<float>(dt), *this);
            accumulator -= dt;

            if (!m_is_running) break;
        }

        m_renderer->beginFrame();
        game.render(*this);
        m_renderer->endFrame();

        m_event_bus->clear();
    }
}
}  // namespace Engine