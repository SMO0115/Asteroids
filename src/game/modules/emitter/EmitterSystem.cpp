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


void EmitterSystem::update(float deltaTime, GameEventBus& event_bus, std::vector<std::unique_ptr<GameObject> > &game_objects) {




    for (const EmitterEvent& event : event_bus.getEvents<EmitterEvent>()) {
        for (auto &object : game_objects) {

            if (object->isActive()) continue;
            if (!object->hasComponent<EmitterComponent>()) continue;
            if (!object->hasComponent<AnimationComponent>()) continue;
            if (!object->hasComponent<PhysicsComponent>()) continue;
            if (!object->hasComponent<TransformComponent>()) continue;
            if (!object->hasComponent<ColliderComponent>()) continue;

            EmitterComponent& emitter       = object->getComponent<EmitterComponent>();
            AnimationComponent& animation   = object->getComponent<AnimationComponent>();
            TransformComponent& transform   = object->getComponent<TransformComponent>();
            PhysicsComponent& physics       = object->getComponent<PhysicsComponent>();
            ColliderComponent& collider     = object->getComponent<ColliderComponent>();
            HealthComponent& health         = object->getComponent<HealthComponent>();


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
        if (!object->hasComponent<EmitterComponent>()) continue;

        EmitterComponent& emitter   = object->getComponent<EmitterComponent>();
        emitter.cooldown_timer     -= deltaTime;

        if (emitter.cooldown_timer <= 0.0f) object->setActive(false);
    }

}
