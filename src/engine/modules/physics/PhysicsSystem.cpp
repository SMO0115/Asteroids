//
// Created by marcel on 11/9/25.
//

#include "engine/core/CoreModule.h"

#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

void PhysicsSystem::update(float deltatime, std::vector<std::unique_ptr<GameObject> > &game_objects) {



    for (const auto &object : game_objects) {

        if (!object->hasComponent<PhysicsComponent>()) continue;
        if (!object->hasComponent<TransformComponent>()) continue;

        PhysicsComponent&   physics   = object->getComponent<PhysicsComponent>();
        TransformComponent& transform = object->getComponent<TransformComponent>();

        transform.position += physics.velocity;
    }
}
