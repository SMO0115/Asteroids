//
// Created by smo0115 on 10/24/26.
//

#include "CombatSystem.h"

#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>

#include "engine/core/CoreModule.h"
#include "engine/events/EngineEvents.h"
#include "engine/events/EventBus.h"
#include "engine/modules/physics/PhysicsComponent.h"
#include "engine/utils/Random.h"
#include "game/core/components/Tags.h"
#include "game/events/GameEvents.h"
#include "game/modules/asteroid/AsteroidComponent.h"
#include "game/modules/player/ShipComponent.h"
#include "game/modules/saucer/SaucerComponent.h"

namespace Game::Combat {

// The two fragments fly off to the left and right of the parent's direction, a bit faster
constexpr float SPLIT_ANGLE_MIN = 20.0f;  // degrees
constexpr float SPLIT_ANGLE_MAX = 60.0f;  // degrees
constexpr float SPLIT_SPEEDUP   = 1.3f;
constexpr float MIN_SPLIT_SPEED = 60.0f;  // px/s, so even a slow asteroid's fragments drift apart


void CombatSystem::update(Engine::Core::Context& ctx) {

    auto& event_bus = ctx.get<Engine::Events::EventBus>();

    for (const auto& collision : event_bus.get<Engine::Events::CollisionEvent>()) {

        Engine::Core::GameObject& a = *collision.from;
        Engine::Core::GameObject& b = *collision.to;

        // Each pair is reported from both sides (A -> B and B -> A), and one bullet can touch two
        // asteroids in the same tick. kill_() switches both off, so whatever was already used up
        // in this tick is skipped here: every hit counts exactly once.
        if (!a.isActive() || !b.isActive()) continue;

        kill_(ctx, a, b);
        kill_(ctx, b, a);
    }
}


void CombatSystem::kill_(Engine::Core::Context& ctx, Engine::Core::GameObject& victim, const Engine::Core::GameObject& killer) {

    auto&           event_bus = ctx.get<Engine::Events::EventBus>();
    const glm::vec2 position  = victim.getComponent<Engine::Core::TransformComponent>().position;

    // Only what the player destroys scores: an asteroid shot by a saucer is worth nothing
    const bool by_player = killer.hasComponent<Core::BulletTag>() || killer.hasComponent<Player::ShipComponent>();

    if (victim.hasComponent<Asteroid::AsteroidComponent>()) {

        event_bus.publish<Events::AsteroidDestroyedEvent>(position, victim.getComponent<Asteroid::AsteroidComponent>().size, by_player);
        split_(ctx, victim);

        // Not erased right now: other events of this tick may still point at it.
        // The engine erases it at the end of the tick (Scene::removeDestroyed).
        victim.destroy();

    } else if (victim.hasComponent<Saucer::SaucerComponent>()) {

        event_bus.publish<Events::SaucerDestroyedEvent>(position, victim.getComponent<Saucer::SaucerComponent>().small, by_player);
        victim.destroy();

    } else if (victim.hasComponent<Player::ShipComponent>()) {

        // Only switched off: the PlayerSystem brings it back (or not, if that was the last life)
        event_bus.publish<Events::ShipDestroyedEvent>(position);
        victim.setActive(false);

    } else {
        victim.setActive(false);  // a bullet (the player's or a saucer's): back to its pool for reuse
    }
}


void CombatSystem::split_(Engine::Core::Context& ctx, Engine::Core::GameObject& asteroid) {

    const int size = asteroid.getComponent<Asteroid::AsteroidComponent>().size;
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
        // where and how big. Spawning is safe here: we loop over the event list, not over the pool that grows.
        scripts.call("SpawnAsteroid", position.x, position.y, child_velocity.x, child_velocity.y, size - 1);
    }
}
}  // namespace Game::Combat
