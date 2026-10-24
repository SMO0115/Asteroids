//
// Created by smo0115 on 10/24/26.
//

#include "SaucerSystem.h"

#include <algorithm>
#include <cmath>
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "SaucerComponent.h"
#include "engine/core/CoreModule.h"
#include "engine/core/Log.h"
#include "engine/core/components/LifeComponent.h"
#include "engine/events/EngineEvents.h"
#include "engine/events/EventBus.h"
#include "engine/modules/graphics/RenderSystem.h"
#include "engine/modules/physics/PhysicsComponent.h"
#include "engine/utils/Random.h"
#include "game/core/GameSession.h"
#include "game/core/GameTypes.h"
#include "game/modules/asteroid/AsteroidComponent.h"
#include "game/modules/player/PlayerSystem.h"

namespace Game::Saucer {

constexpr float SPAWN_DELAY_FIRST = 20.0f;  // seconds before a saucer in the first minutes ...
constexpr float SPAWN_DELAY_STEP  = 3.0f;   // ... this much less per 10,000 points ...
constexpr float SPAWN_DELAY_MIN   = 8.0f;   // ... but never less than this
constexpr float SPAWN_DELAY_RAND  = 4.0f;   // plus 0..this, so they don't come like clockwork

constexpr float LARGE_SPEED = 100.0f;  // px/s, left-right
constexpr float SMALL_SPEED = 150.0f;
constexpr float ZIGZAG      = 0.6f;   // up-down speed, as a part of the left-right speed
constexpr float HALF_WIDTH  = 48.0f;  // px: half the 96 px sprite (screenWrapper's margin is the same)

constexpr float FIRE_INTERVAL   = 1.0f;    // seconds between two shots
constexpr float BULLET_SPEED    = 350.0f;  // px/s
constexpr float BULLET_LIFETIME = 1.2f;    // seconds

// The small saucer aims at the ship, but misses by up to this many degrees: less as the score grows
constexpr float AIM_ERROR_START = 20.0f;
constexpr float AIM_ERROR_MIN   = 4.0f;


namespace {
// Seconds until the next saucer, at this score
float spawnDelay(int score) {
    const float delay = SPAWN_DELAY_FIRST - SPAWN_DELAY_STEP * static_cast<float>(score) / 10000.0f;
    return std::max(delay, SPAWN_DELAY_MIN) + Engine::Core::Random::getFloat(0.0f, SPAWN_DELAY_RAND);
}

// The chance that the next saucer is the small one: 20% at the start, always from 32,000 points
float smallChance(int score) { return std::clamp(0.2f + static_cast<float>(score) / 40000.0f, 0.2f, 1.0f); }
}  // namespace


void SaucerSystem::reset() { m_spawn_timer = spawnDelay(0); }


void SaucerSystem::update(Engine::Core::Context& ctx, float dt) {

    bool saucer_flying  = false;
    bool asteroids_left = false;

    for (const auto& object : ctx.get<Engine::Core::Scene>().getPool(Game::Core::GamePools::ENEMIES)) {

        if (!object->isActive()) continue;

        if (object->hasComponent<SaucerComponent>()) {
            saucer_flying = true;
            fly_(ctx, *object, dt);  // may destroy() it, but never erases or adds to this pool
        } else if (object->hasComponent<Asteroid::AsteroidComponent>()) {
            asteroids_left = true;
        }
    }

    // One saucer at a time, only during a wave, and only while there is a ship to shoot at
    if (saucer_flying || !asteroids_left || !Player::PlayerSystem::shipInPlay(ctx)) return;

    m_spawn_timer -= dt;
    if (m_spawn_timer > 0.0f) return;

    spawn_(ctx);
    m_spawn_timer = spawnDelay(ctx.get<Game::Core::GameSession>().score);
}


void SaucerSystem::removeAll(Engine::Core::Context& ctx) {

    auto& scene = ctx.get<Engine::Core::Scene>();

    for (const auto& object : scene.getPool(Game::Core::GamePools::ENEMIES)) {
        if (object->hasComponent<SaucerComponent>()) object->destroy();
    }
    for (const auto& bullet : scene.getPool(Game::Core::GamePools::SAUCER_BULLETS)) bullet->setActive(false);
}


void SaucerSystem::spawn_(Engine::Core::Context& ctx) {

    using Engine::Core::Random;

    const glm::vec2 screen = glm::vec2(ctx.get<Engine::Graphics::RenderSystem>().getScreenSize());
    const bool      small  = Random::getFloat(0.0f, 1.0f) < smallChance(ctx.get<Game::Core::GameSession>().score);

    // Just outside the left or the right edge, flying to the other side, at a random height
    const bool  from_left = Random::getInt(0, 1) == 0;
    const float x         = from_left ? -HALF_WIDTH : screen.x + HALF_WIDTH;
    const float y         = Random::getFloat(0.15f, 0.85f) * screen.y;
    const float speed     = (small ? SMALL_SPEED : LARGE_SPEED) * (from_left ? 1.0f : -1.0f);

    ctx.get<Engine::Scripting::ScriptManager>().call("SpawnSaucer", x, y, speed, small);
    LOG_INFO("A {} saucer appears", small ? "small" : "large");
}


void SaucerSystem::fly_(Engine::Core::Context& ctx, Engine::Core::GameObject& saucer, float dt) {

    using Engine::Core::Random;

    auto& state   = saucer.getComponent<SaucerComponent>();
    auto& physics = saucer.getComponent<Engine::Physics::PhysicsComponent>();

    // 1. Leave after crossing the screen once. Counting the distance works although the screen wraps
    // (the screenWrapper would bring it back on the other side, to fly forever).
    const float screen_width = static_cast<float>(ctx.get<Engine::Graphics::RenderSystem>().getScreenSize().x);
    state.travelled += std::abs(physics.velocity.x) * dt;
    if (state.travelled >= screen_width + 2.0f * HALF_WIDTH) {
        saucer.destroy();  // no points, no bang: it just flew away
        return;
    }

    // 2. Zig-zag: now and then switch between flying up, straight and down
    state.turn_timer -= dt;
    if (state.turn_timer <= 0.0f) {
        state.turn_timer   = Random::getFloat(0.8f, 2.0f);
        physics.velocity.y = static_cast<float>(Random::getInt(-1, 1)) * std::abs(physics.velocity.x) * ZIGZAG;
    }

    // 3. Shoot
    state.fire_timer -= dt;
    if (state.fire_timer <= 0.0f) {
        state.fire_timer = FIRE_INTERVAL;
        fire_(ctx, saucer);
    }
}


void SaucerSystem::fire_(Engine::Core::Context& ctx, Engine::Core::GameObject& saucer) {

    using Engine::Core::Random;

    // A free slot in the saucer bullet pool (2 slots, like the arcade). None free: no shot this time.
    auto& bullets = ctx.get<Engine::Core::Scene>().getPool(Game::Core::GamePools::SAUCER_BULLETS);
    auto  free    = std::find_if(bullets.begin(), bullets.end(), [](const std::unique_ptr<Engine::Core::GameObject>& b) { return !b->isActive(); });
    if (free == bullets.end()) return;
    Engine::Core::GameObject& bullet = **free;

    const glm::vec2 position = saucer.getComponent<Engine::Core::TransformComponent>().position;

    // The large saucer shoots anywhere. The small one aims at the ship, missing by a few degrees
    // (fewer as the score grows).
    float angle = Random::getFloat(0.0f, 360.0f);

    if (saucer.getComponent<SaucerComponent>().small && Player::PlayerSystem::shipInPlay(ctx)) {

        Engine::Core::GameObject& ship = *ctx.get<Engine::Core::Scene>().getPool(Game::Core::GamePools::PLAYER)[0];  // shipInPlay: it exists

        const glm::vec2 to_ship = ship.getComponent<Engine::Core::TransformComponent>().position - position;
        const float     score   = static_cast<float>(ctx.get<Game::Core::GameSession>().score);
        const float     error   = std::max(AIM_ERROR_START - score / 2000.0f, AIM_ERROR_MIN);

        angle = glm::degrees(std::atan2(to_ship.y, to_ship.x)) + Random::getFloat(-error, error);
    }

    bullet.getComponent<Engine::Core::TransformComponent>().position  = position;
    bullet.getComponent<Engine::Physics::PhysicsComponent>().velocity = glm::rotate(glm::vec2(BULLET_SPEED, 0.0f), glm::radians(angle));
    bullet.getComponent<Engine::Core::LifeComponent>().life_time      = BULLET_LIFETIME;
    bullet.setActive(true);

    ctx.get<Engine::Events::EventBus>().publish<Engine::Events::PlaySoundEvent>("fire");
}
}  // namespace Game::Saucer
