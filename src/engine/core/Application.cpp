//
// Created by marcel on 7/31/25.
//

#include "Application.h"


#include "engine/core/modules/LifeModule.h"
#include "engine/modules/assets/AssetModule.h"
#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/graphics/RenderModule.h"
#include "engine/modules/input/InputModule.h"
#include "engine/modules/physics/PhysicsModule.h"

#include "Log.h"
#include "interfaces/IGame.h"

#include "Scene.h"
#include "engine/core/GameObject.h"


#include "engine/core/Context.h"
#include "engine/core/TimeManager.h"
#include "engine/events/EngineEvents.h"
#include "engine/events/EventBus.h"
#include "engine/scripting/ComponentRegistry.h"
#include "engine/scripting/ScriptManager.h"
#include "engine/scripting/StateRegistry.h"
#include "engine/scripting/bindings/ComponentBinding.h"

#include "engine/core/components/TransformComponent.h"


namespace Engine {
Application::Application() {

    m_is_running = false;


    m_context = std::make_unique<Engine::Core::Context>();

    // Order matters: the Context destroys services in REVERSE order, so a service may use
    // everything registered above it (the AssetManager needs the renderer to free textures)
    m_context->registerService<Core::TimeManager>();
    m_context->registerService<Input::InputManager>();
    m_context->registerService<Core::Scene>();
    m_context->registerService<Graphics::RenderSystem>();
    m_context->registerService<Graphics::AnimationSystem>();
    m_context->registerService<Audio::SoundSystem>();
    m_context->registerService<Physics::PhysicsSystem>();
    m_context->registerService<Physics::CollisionSystem>();
    m_context->registerService<Core::LifeSystem>();
    m_context->registerService<Assets::AssetManager>(&m_context->get<Graphics::RenderSystem>());
    m_context->registerService<Events::EventBus>();
    m_context->registerService<Scripting::StateRegistry>();
    m_context->registerService<Scripting::ComponentRegistry>();
    m_context->registerService<Scripting::ScriptManager>(*m_context);

    // The engine's own components. The game adds its components and names in IGame::init(),
    // before it runs any script that uses them.
    Scripting::registerEngineComponents(m_context->get<Scripting::ComponentRegistry>(), m_context->get<Scripting::StateRegistry>());
}

Application::~Application() = default;

// target_fps isn't used yet: vsync paces the frames
bool Application::init(const std::string& title, std::size_t width, std::size_t height, glm::uint32_t /*target_fps*/) {

    if (!m_context->get<Graphics::RenderSystem>().init(title, width, height)) return false;
    if (!m_context->get<Audio::SoundSystem>().init()) return false;

    return true;
}


void Application::run(Core::IGame& game) {

    if (!game.init(*m_context)) return;
    m_is_running = true;

    Core::TimeManager&      time_manager  = m_context->get<Core::TimeManager>();
    Graphics::RenderSystem& renderer      = m_context->get<Graphics::RenderSystem>();
    Input::InputManager&    input_manager = m_context->get<Input::InputManager>();
    Events::EventBus&       event_bus     = m_context->get<Events::EventBus>();
    Core::Scene&            scene         = m_context->get<Core::Scene>();

    event_bus.registerEvent<Engine::Events::CollisionEvent>();

    time_manager.start();
    constexpr double dt          = 1.0 / 120.0;
    double           accumulator = 0.0;


    while (m_is_running) {

        double frameTime = time_manager.tick();
        if (frameTime > 0.25) frameTime = 0.25;

        accumulator += frameTime;


        // Everything that is an *event* (key presses, collisions, ...) must be produced,
        // consumed and cleared on the same clock. A frame can run 0..N ticks, so all of
        // it lives inside the fixed-step loop.
        while (accumulator >= dt) {

            input_manager.processInput();
            if (input_manager.isKeyHeld(Input::Key::QUIT)) m_is_running = false;
            game.handleInput(*m_context);

            m_is_running = game.update(static_cast<float>(dt), *m_context) && m_is_running;
            event_bus.clear();

            // Only now, with the events (raw GameObject pointers) gone, is it safe to erase objects
            scene.removeDestroyed();
            accumulator -= dt;

            if (!m_is_running) break;
        }

        renderer.beginFrame();
        game.render(*m_context);
        renderer.endFrame();
    }
}
}  // namespace Engine