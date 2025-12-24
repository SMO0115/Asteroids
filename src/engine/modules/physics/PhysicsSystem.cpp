//
// Created by marcel on 11/9/25.
//

#include "engine/core/CoreModule.h"

#include "PhysicsComponent.h"
#include "PhysicsSystem.h"

namespace Engine::Physics {
void PhysicsSystem::update(float deltatime, std::vector<std::unique_ptr<Core::GameObject> >& game_objects) {


    for (const auto& object : game_objects) {

        if (!object->hasComponent<PhysicsComponent>()) continue;
        if (!object->hasComponent<Core::TransformComponent>()) continue;

        PhysicsComponent&         physics   = object->getComponent<PhysicsComponent>();
        Core::TransformComponent& transform = object->getComponent<Core::TransformComponent>();

        transform.position += physics.velocity;
    }
}
}  // namespace Engine::Physics