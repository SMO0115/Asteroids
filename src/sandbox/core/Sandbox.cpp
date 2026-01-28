//
// Created by marcel on 9/28/25.
//

#include "glm/glm.hpp"


#include "Sandbox.h"

#include "engine/core/CoreModule.h"

#include "engine/modules/assets/AssetModule.h"
#include "engine/modules/audio/AudioModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/graphics/RenderModule.h"

#include "game/modules/control/ControlModul.h"
#include "game/modules/emitter/EmiterModule.h"
#include "game/modules/health/HealthModule.h"
#include "game/modules/ui/UIModule.h"

#include "GameTypes.h"

#include "game/modules/control/InvaderComponent.h"
#include "game/modules/control/PlayerComponent.h"

#include "components/Tags.h"

#include "game/events/GameEvents.h"

#include "game/states/RunState.h"


struct Entities {};

namespace Game {
    Sandbox::Sandbox() {


        m_event_bus             = std::make_unique<Events::GameEventBus>();

    }

    Sandbox::~Sandbox() {}


    bool Sandbox::init(Engine::Application& engine) {

        engine.getScriptManager().runFile("assets/scripts/asset_loader.lua");


        Engine::Core::Scene& scene = engine.getScene();
        Pool& entities = scene.registerPool<Entities>();



        return true;
    }


    void Sandbox::handleInput(Engine::Input::InputManager& input_manager) {


    }


    bool Sandbox::update(float deltatime, Engine::Application& engine) {



        Engine::Core::Scene& scene = engine.getScene();

        if (scene.hasPool<Entities>()) {
            std::printf("ent exists \n");

            Pool& entities = scene.getPool<Entities>();


            std::unique_ptr<Engine::Core::GameObject> player = std::make_unique<Engine::Core::GameObject>();

            player->addComponent<Engine::Core::TransformComponent>();
            player->getComponent<Engine::Core::TransformComponent>().position = glm::vec2(400, 750);
            player->getComponent<Engine::Core::TransformComponent>().scale    = 2;
            player->addComponent<Engine::Physics::PhysicsComponent>();
            player->addComponent<Engine::Graphics::SpriteComponent>();
            player->addComponent<Control::InputComponent>();
            player->addComponent<Control::PlayerComponent>();

            player->addComponent<Health::HealthComponent>();
            player->getComponent<Health::HealthComponent>().max_health     = 100;
            player->getComponent<Health::HealthComponent>().current_health = 100;

            entities.push_back(std::move(player));
        }



        m_event_bus->clear();

        return true;
    }

    void Sandbox::render(Engine::Application& engine) {

    }

}  // namespace Game
