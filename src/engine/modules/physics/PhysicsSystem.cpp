//
// Created by marcel on 11/9/25.
//

#include <cmath>

#include "engine/core/CoreModule.h"

#include "PhysicsComponent.h"
#include "PhysicsSystem.h"

namespace Engine::Physics {
void PhysicsSystem::update(Engine::Core::Context& ctx, float dt) {

    auto game_objects = ctx.get<Engine::Core::Scene>().getAll();

    for (auto& object : game_objects) {

        if (!object.isActive()) continue;  // pooled bullets and destroyed objects don't move
        if (!object.hasComponent<PhysicsComponent>()) continue;
        if (!object.hasComponent<Core::TransformComponent>()) continue;

        PhysicsComponent&         physics   = object.getComponent<PhysicsComponent>();
        Core::TransformComponent& transform = object.getComponent<Core::TransformComponent>();

        // damping = fraction of velocity kept per second, so the result doesn't depend on the tick rate
        physics.velocity        = physics.velocity * std::pow(physics.damping, dt) + physics.acceleration * dt;
        physics.angularVelocity = physics.angularVelocity * std::pow(physics.angularDamping, dt) + physics.angularAcceleration * dt;

        transform.position += physics.velocity * dt;
        transform.rotation += physics.angularVelocity * dt;
    }
}
}  // namespace Engine::Physics