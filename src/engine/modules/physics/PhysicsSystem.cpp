//
// Created by marcel on 11/9/25.
//

#include "engine/core/CoreModule.h"

#include "PhysicsComponent.h"
#include "PhysicsSystem.h"

namespace Engine::Physics {
void PhysicsSystem::update(Engine::Core::Context& ctx) {

    float deltatime    = static_cast<float>(1.0f / 120.0f);
    auto  game_objects = ctx.get<Engine::Core::Scene>().getAll();

    for (auto& object : game_objects) {

        if (!object.hasComponent<PhysicsComponent>()) continue;
        if (!object.hasComponent<Core::TransformComponent>()) continue;

        PhysicsComponent&         physics   = object.getComponent<PhysicsComponent>();
        Core::TransformComponent& transform = object.getComponent<Core::TransformComponent>();

        physics.velocity = physics.damping * physics.velocity + deltatime * physics.acceleration;
        physics.angularVelocity = physics.angularDamping * physics.angularVelocity +  physics.angularAcceleration;

        transform.position += physics.velocity * deltatime;
        transform.rotation += physics.angularVelocity * deltatime;
    }
}
}  // namespace Engine::Physics