//
// Created by marcel on 11/23/25.
//

#include "HealthSystem.h"

#include "game/core/CoreModule.h"
#include "HealthComponent.h"
#include "LifetimeComponent.h"
#include "../emitter/EmitterComponent.h"


#include "engine/core/CoreModule.h"
#include "engine/modules/rendering/RenderModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/assets/AssetModule.h"
#include "engine/events/EngineEvents.h"

#include <iostream>

void HealthSystem::update(float deltaTime, EngineEventBus &event_bus, std::vector<std::unique_ptr<GameObject> > &game_objects) {

    for (const CollisionEvent& event : event_bus.getEvents<CollisionEvent>()) {

        if (!event.to->hasComponent<HealthComponent>()) continue;
        event.to->getComponent<HealthComponent>().current_health -= 100;

        if (event.to->hasComponent<WallComponent>()) {

            int state = static_cast<int>( event.to->getComponent<HealthComponent>().current_health / 100 );
            std::cout<<"state: "<<state<<std::endl;
            event.to->getComponent<AnimationComponent>().animations[0]->frames[0] = {11*(4 - state), 0, 11, 8};;
        }

        if (event.to->getComponent<HealthComponent>().current_health <= 0) {


            if (event.to->hasComponent<EmitterComponent>()) {
                event.to->setActive(false);
                continue;
            }


            event.to->getComponent<AnimationComponent>().current_state =  static_cast<int>( AnimationState::DEATH );

            event.to->removeComponent<ColliderComponent>();
            event.to->addComponent<LifetimeComponent>(0.1);

            event_bus.publish<PlaySoundEvent>("explosion");
        }
    }
}
