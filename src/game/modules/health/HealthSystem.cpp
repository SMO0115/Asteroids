//
// Created by marcel on 11/23/25.
//

#include "HealthSystem.h"

#include "HealthComponent.h"
#include "LifetimeComponent.h"
#include "game/core/CoreModule.h"
#include "game/modules/emitter/EmitterComponent.h"

#include "engine/core/CoreModule.h"
#include "engine/events/EngineEvents.h"
#include "engine/modules/assets/AssetModule.h"
#include "engine/modules/physics/PhysicsModule.h"
#include "engine/modules/rendering/RenderModule.h"

#include <iostream>

namespace Game::Health {
void HealthSystem::update(float deltaTime, Engine::Events::EngineEventBus& event_bus, std::vector<std::unique_ptr<Engine::Core::GameObject> >& game_objects) {
    for (const Engine::Events::CollisionEvent& event : event_bus.getEvents<Engine::Events::CollisionEvent>()) {
        if (!event.to->hasComponent<HealthComponent>()) continue;
        event.to->getComponent<Health::HealthComponent>().current_health -= 100;

        if (event.to->hasComponent<Core::WallComponent>()) {
            int state = static_cast<int>(event.to->getComponent<Health::HealthComponent>().current_health / 100);
            std::cout << "Health: " << state << std::endl;
            event.to->getComponent<Engine::Graphics::AnimationComponent>().animations[0]->frames[0] = {11 * (4 - state), 0, 11, 8};
            ;
        }

        if (event.to->getComponent<Health::HealthComponent>().current_health <= 0) {
            if (event.to->hasComponent<Emitter::EmitterComponent>()) {
                event.to->setActive(false);
                continue;
            }

            event.to->getComponent<Engine::Graphics::AnimationComponent>().current_state = static_cast<int>(Core::AnimationState::DEATH);

            event.to->removeComponent<Engine::Physics::ColliderComponent>();
            event.to->addComponent<Health::LifetimeComponent>(0.1);

            event_bus.publish<Engine::Events::PlaySoundEvent>("explosion");
        }
    }
}
}  // namespace Game::Health