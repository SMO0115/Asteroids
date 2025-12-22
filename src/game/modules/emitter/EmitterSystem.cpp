//
// Created by marcel on 11/17/25.
//

#include "EmitterSystem.h"

#include "engine/core/CoreModule.h"
#include "engine/modules/rendering/RenderModule.h"
#include "engine/modules/physics/PhysicsModule.h"

#include "game/modules/emitter/EmitterComponent.h"
#include "game/modules/health/HealthModule.h"


#include "../../../engine/events/EventBus.h"
#include "../../events/GameEvents.h"


namespace Game::Emitter {
    void EmitterSystem::update(float deltaTime, Events::GameEventBus& event_bus, std::vector<std::unique_ptr<Engine::Core::GameObject> > &game_objects) {




        for (const Events::EmitterEvent& event : event_bus.getEvents<Events::EmitterEvent>()) {
            for (auto &object : game_objects) {

                if (object->isActive()) continue;
                if (!object->hasComponent<Emitter::EmitterComponent>()) continue;
                if (!object->hasComponent<Engine::Graphics::AnimationComponent>()) continue;
                if (!object->hasComponent<Engine::Physics::PhysicsComponent>()) continue;
                if (!object->hasComponent<Engine::Core::TransformComponent>()) continue;
                if (!object->hasComponent<Engine::Physics::ColliderComponent>()) continue;

                EmitterComponent& emitter       = object->getComponent<Emitter::EmitterComponent>();
                Engine::Graphics::AnimationComponent& animation   = object->getComponent<Engine::Graphics::AnimationComponent>();
                Engine::Core::TransformComponent& transform   = object->getComponent<Engine::Core::TransformComponent>();
                Engine::Physics::PhysicsComponent& physics       = object->getComponent<Engine::Physics::PhysicsComponent>();
                Engine::Physics::ColliderComponent& collider     = object->getComponent<Engine::Physics::ColliderComponent>();
                Health::HealthComponent& health         = object->getComponent<Health::HealthComponent>();


                object->setActive(true);

                health.max_health           = 100;
                health.current_health       = 100;

                animation.current_state     = event.emitter_animation_id;
                animation.current_frame     = 0;
                animation.current_duration  = 0.0f;

                collider.layer              = event.layer;
                collider.mask               = event.mask;

                transform.position          = event.position;

                physics.velocity            = event.velocity;

                emitter.cooldown_timer      = emitter.cooldown;

                break;
            }
        }


        for (auto &object : game_objects) {


            if (!object->isActive()) continue;
            if (!object->hasComponent<Emitter::EmitterComponent>()) continue;

            Emitter::EmitterComponent& emitter   = object->getComponent<Emitter::EmitterComponent>();
            emitter.cooldown_timer     -= deltaTime;

            if (emitter.cooldown_timer <= 0.0f) object->setActive(false);
        }

    }
}