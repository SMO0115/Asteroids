//
// Created by smo0115 on 10/16/26.
//

#include "AsteroidSystem.h"

#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>

#include "AsteroidComponent.h"
#include "engine/core/CoreModule.h"
#include "engine/events/EngineEvents.h"
#include "engine/events/EventBus.h"
#include "engine/modules/physics/PhysicsComponent.h"
#include "engine/utils/Random.h"
#include "game/events/GameEvents.h"
#include "game/modules/player/ShipComponent.h"

namespace Game::Asteroid {

// The two fragments fly off to the left and right of the parent's direction, a bit faster
constexpr float SPLIT_ANGLE_MIN = 20.0f;  // degrees
constexpr float SPLIT_ANGLE_MAX = 60.0f;  // degrees
constexpr float SPLIT_SPEEDUP   = 1.3f;
constexpr float MIN_SPLIT_SPEED = 60.0f;  // px/s, so even a slow asteroid's fragments drift apart


void AsteroidSystem::update(Engine::Core::Context& ctx) {

    auto& event_bus = ctx.get<Engine::Events::EventBus>();

    // Every overlapping pair is reported from both sides (bullet -> asteroid AND asteroid -> bullet).
    // We only react to "something hit an asteroid", so each hit is handled exactly once.
    for (const auto& collision : event_bus.get<Engine::Events::CollisionEvent>()) {

        Engine::Core::GameObject& hitter   = *collision.from;
        Engine::Core::GameObject& asteroid = *collision.to;

        if (!asteroid.hasComponent<AsteroidComponent>()) continue;

        // Already used up earlier in this tick: two bullets hit the same asteroid,
        // or one bullet touched two asteroids at once
        if (!asteroid.isActive() || !hitter.isActive()) continue;

        // A bullet goes back to its pool for reuse. The ship is switched off too: the PlayerSystem
        // brings it back (or not, if that was the last life). Switching off right away is also what
        // makes a second hit in this tick get skipped above.
        hitter.setActive(false);

        if (hitter.hasComponent<Player::ShipComponent>()) {
            event_bus.publish<Events::ShipDestroyedEvent>(hitter.getComponent<Engine::Core::TransformComponent>().position);
        }

        event_bus.publish<Events::AsteroidDestroyedEvent>(asteroid.getComponent<Engine::Core::TransformComponent>().position,
                                                          asteroid.getComponent<AsteroidComponent>().size);

        split_(ctx, asteroid);

        // Not erased right now: other events of this tick may still point at it.
        // The engine erases it at the end of the tick (Scene::removeDestroyed).
        asteroid.destroy();
    }
}


void AsteroidSystem::split_(Engine::Core::Context& ctx, Engine::Core::GameObject& asteroid) {

    const int size = asteroid.getComponent<AsteroidComponent>().size;
    if (size <= 1) return;  // small asteroids just disappear

    const glm::vec2 position = asteroid.getComponent<Engine::Core::TransformComponent>().position;
    const glm::vec2 velocity = asteroid.getComponent<Engine::Physics::PhysicsComponent>().velocity;

    const float     parent_speed = glm::length(velocity);
    const glm::vec2 direction    = parent_speed > 0.0f ? velocity / parent_speed : glm::vec2(1.0f, 0.0f);
    const float     speed        = std::max(parent_speed * SPLIT_SPEEDUP, MIN_SPLIT_SPEED);

    auto& scripts = ctx.get<Engine::Scripting::ScriptManager>();

    for (const float side : {-1.0f, 1.0f}) {

        const float     angle          = side * Engine::Core::Random::getFloat(SPLIT_ANGLE_MIN, SPLIT_ANGLE_MAX);
        const glm::vec2 child_velocity = glm::rotate(direction, glm::radians(angle)) * speed;

        // The asteroid "prefab" (sprite, collider, ...) is defined once, in Lua, and C++ only says
        // where and how big. Before, it was copy-pasted here and could drift away from the Lua one.
        // Spawning is safe here: we loop over the event list, not over the pool that grows.
        scripts.call("SpawnAsteroid", position.x, position.y, child_velocity.x, child_velocity.y, size - 1);
    }
}
}  // namespace Game::Asteroid
