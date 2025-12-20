//
// Created by marcel on 12/6/25.
//

#include "LifetimeSystem.h"

#include "engine/core/CoreModule.h"
#include  "game/modules/health/LifetimeComponent.h"


void LifetimeSystem::update(float deltaTime, GameEventBus& event_bus, std::vector<std::unique_ptr<GameObject> > &game_objects) {

    for (auto &object : game_objects) {

        if (!object->isActive()) continue;
        if (!object->hasComponent<LifetimeComponent>()) continue;

        LifetimeComponent& life = object->getComponent<LifetimeComponent>();
        life.time_left -= deltaTime;

         if (life.time_left <= 0) {
             object->setActive(false);
             event_bus.publish<DeathEvent>(object.get());
        }
    }
}