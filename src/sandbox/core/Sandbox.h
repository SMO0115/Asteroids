//
// Created by marcel on 9/28/25.
//

#pragma once

#include "engine/core/CoreModule.h"
#include "game/events/GameEvents.h"

#include <memory>

namespace Engine { class Application; }
namespace Engine::Core { class GameObject; }
namespace Engine::Core { class IGameState; }
namespace Engine::Core {class IGame; }
namespace Engine::Input { class InputManager; }
namespace Engine::Graphics { class RenderSystem; }
namespace Game::Control { class PlayerControlSystem; }
namespace Game::Control { class AISystem; }
namespace Game::Emitter { class EmitterSystem; }
namespace Game::Health { class HealthSystem; }
namespace Game::Health { class LifetimeSystem; }
namespace Game::UI { class ScoreSystem; }
namespace Game::UI { class UISystem; }

namespace Game {
    class Sandbox : public Engine::Core::IGame {

    public:
        Sandbox();
        ~Sandbox() override;

        bool init(Engine::Application& engine) override;
        void handleInput(Engine::Input::InputManager& input_manager) override;
        bool update(float deltatime, Engine::Application& engine) override;
        void render(Engine::Application& engine) override;

    private:

        std::unique_ptr<Events::GameEventBus>   m_event_bus;
    };
}  // namespace Game