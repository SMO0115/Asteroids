//
// Created by marcel on 7/31/25.
//

#include "Engine.h"
#include <iostream>
#include <bits/stdc++.h>


#include "engine/modules/rendering/RenderModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/input/InputModule.h"
#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/assets/AssetModule.h"

#include "IGame.h"



#include "../core/TimeManager.h"
#include "../events/EventBus.h"




Engine::Engine() {

    m_is_running        = 0;
    m_quit_requested    = 0;

    m_asset_manager     = std::make_unique<AssetManager>();
    m_input_manager     = std::make_unique<InputManager>();
    m_time_manager      = std::make_unique<TimeManager>();

    m_renderer          = std::make_unique<RenderSystem>();
    m_sound_system      = std::make_unique<SoundSystem>();
    m_animation_system  = std::make_unique<AnimationSystem>();
    m_physics           = std::make_unique<PhysicsSystem>();
    m_collision_system  = std::make_unique<CollisionSystem>();

    m_event_bus         = std::make_unique<EngineEventBus>();
}

Engine::~Engine() = default;

bool Engine::init(const std::string& title, std::size_t width, std::size_t height, glm::uint32_t target_fps) {

    if (!m_renderer->init(title, width, height)) return false;
    if (!m_sound_system->init()) return false;

    return true;
}


void Engine::run(IGame &game) {

    if (!game.init(*this)) return;
    m_is_running = true;

    m_time_manager->start();
    constexpr double dt = 1.0 / 120.0;
    double accumulator = 0.0;

    while (m_is_running) {

        double frameTime = m_time_manager->tick();
        if (frameTime > 0.25) frameTime = 0.25;

        accumulator += frameTime;

        m_input_manager->processInput();
        game.handleInput(*m_input_manager);
        if (m_input_manager->isKeyHeld(Key::QUIT)) m_is_running = false;


        while (accumulator >= dt) {

            m_is_running = game.update(static_cast<float>(dt), *this);
            accumulator -= dt;

            if (!m_is_running) break;
        }

        m_renderer->beginFrame();
        game.render(*m_renderer);
        m_renderer->endFrame();

        m_event_bus->clear();
    }
}