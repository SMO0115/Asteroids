//
// Created by marcel on 2/10/26.
//

#include "PlayerSystem.h"

#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "ShipComponent.h"
#include "engine/core/CoreModule.h"
#include "engine/core/components/LifeComponent.h"
#include "engine/events/EngineEvents.h"
#include "engine/events/EventBus.h"
#include "engine/modules/audio/SoundComponent.h"
#include "engine/modules/graphics/AnimationComponent.h"
#include "engine/modules/graphics/RenderSystem.h"
#include "engine/modules/input/InputManager.h"
#include "engine/modules/physics/PhysicsComponent.h"
#include "engine/utils/Random.h"
#include "game/core/GameSession.h"
#include "game/core/GameTypes.h"
#include "game/events/GameEvents.h"
#include "game/modules/asteroid/AsteroidComponent.h"

namespace Game::Player {

// Rates are per second; multiply by dt so the feel doesn't depend on the tick rate
constexpr float THRUST     = 240.0f;  // px/s^2  (was 2.0 per tick at 120 Hz)
constexpr float TURN_ACCEL = 180.0f;  // deg/s^2 (was 1.5 per tick at 120 Hz)

constexpr float BULLET_SPEED    = 500.0f;  // px/s, on top of the ship's own speed
constexpr float BULLET_LIFETIME = 1.2f;    // seconds: a shot reaches ~3/4 of the screen, like the arcade

constexpr float HYPERSPACE_TIME   = 0.5f;    // seconds the ship is gone
constexpr float HYPERSPACE_MARGIN = 60.0f;   // px: it never reappears closer than this to an edge
constexpr float HYPERSPACE_RISK   = 0.125f;  // 1 jump in 8 goes wrong, like the arcade's gamble (0 = always safe)

constexpr float RESPAWN_DELAY  = 2.0f;    // seconds between the hit and the new ship (at the earliest)
constexpr float SAFE_RADIUS    = 150.0f;  // px: no asteroid may be this close to the respawn point
constexpr float SPAWN_ROTATION = -90.0f;  // degrees: nose up (the sprite itself points right)


namespace {
// True if no asteroid's center is within 'radius' of 'point'
bool isAreaClear(Engine::Core::Context& ctx, glm::vec2 point, float radius) {

    for (const auto& object : ctx.get<Engine::Core::Scene>().getPool(Game::Core::GamePools::ENEMIES)) {

        if (!object->isActive() || !object->hasComponent<Asteroid::AsteroidComponent>()) continue;

        const glm::vec2 position = object->getComponent<Engine::Core::TransformComponent>().position;
        if (glm::distance(position, point) < radius) return false;
    }
    return true;
}
}  // namespace


void PlayerSystem::update(Engine::Core::Context& ctx, float dt) {

    Engine::Core::Pool& player_pool = ctx.get<Engine::Core::Scene>().getPool(Game::Core::GamePools::PLAYER);

    if (player_pool.empty()) return;  // [0] on an empty vector is undefined behavior
    auto& player = *player_pool[0];
    auto& ship   = player.getComponent<ShipComponent>();

    // Hit in this tick (AsteroidSystem has already switched it off)
    if (!ctx.get<Engine::Events::EventBus>().get<Events::ShipDestroyedEvent>().empty()) {
        ship.status = ShipComponent::Status::DESTROYED;
        ship.timer  = RESPAWN_DELAY;
    }

    switch (ship.status) {
        case ShipComponent::Status::FLYING:
            fly_(ctx, player, ship, dt);
            break;
        case ShipComponent::Status::HYPERSPACE:
            returnFromHyperspace_(ctx, player, ship, dt);
            break;
        case ShipComponent::Status::DESTROYED:
            waitForRespawn_(ctx, player, ship, dt);
            break;
    }
}


void PlayerSystem::fly_(Engine::Core::Context& ctx, Engine::Core::GameObject& player, ShipComponent& ship, float dt) {

    Engine::Input::InputManager&          input     = ctx.get<Engine::Input::InputManager>();
    Engine::Graphics::AnimationComponent& anim      = player.getComponent<Engine::Graphics::AnimationComponent>();
    Engine::Audio::SoundComponent&        sound     = player.getComponent<Engine::Audio::SoundComponent>();
    Engine::Physics::PhysicsComponent&    physics   = player.getComponent<Engine::Physics::PhysicsComponent>();
    Engine::Core::TransformComponent&     transform = player.getComponent<Engine::Core::TransformComponent>();

    // Rotates a vector from "ship space" (+x = where the nose points) into the world.
    // glm::rotate does the same as the hand-written rotation matrix that used to be here.
    const float rotation = glm::radians(transform.rotation);
    auto        toWorld  = [rotation](glm::vec2 v) { return glm::rotate(v, rotation); };

    // No more 'anim.current_duration = 1.0f' here: it forced a new frame on every tick. The flame's
    // speed now comes from the "duration" in player_run.json, and the AnimationSystem restarts the
    // animation by itself when the state changes.
    anim.current_state  = Game::Core::AnimationState::IDLE;
    sound.current_state = Game::Core::SoundState::IDLE;  // the SoundSystem stops the thrust loop when this changes

    // Hyperspace: vanish now, reappear somewhere random in a moment (returnFromHyperspace_). Switched
    // off in between, so it can't be hit, and the SoundSystem stops the thrust.
    if (input.wasKeyPressed(Engine::Input::Key::DOWN)) {
        player.setActive(false);
        ship.status = ShipComponent::Status::HYPERSPACE;
        ship.timer  = HYPERSPACE_TIME;
        return;
    }

    if (input.wasKeyPressed(Engine::Input::Key::SPACE)) {

        // Spawn bullet
        auto& bullets = ctx.get<Engine::Core::Scene>().getPool(Game::Core::GamePools::BULLETS);

        for (auto& b : bullets) {

            if (!b->isActive()) {

                b->setActive(true);

                auto& bullet_transform    = b->getComponent<Engine::Core::TransformComponent>();
                bullet_transform.position = transform.position + toWorld({40.0f, -2.0f});  // at the nose


                auto& bullet_physics    = b->getComponent<Engine::Physics::PhysicsComponent>();
                bullet_physics.velocity = physics.velocity + toWorld({BULLET_SPEED, 0.0f});

                // The LifeSystem counts this down and switches the bullet off at 0
                b->getComponent<Engine::Core::LifeComponent>().life_time = BULLET_LIFETIME;

                ctx.get<Engine::Events::EventBus>().publish<Engine::Events::PlaySoundEvent>("fire");

                break;
            }
        }
    }


    if (input.isKeyHeld(Engine::Input::Key::UP)) {

        physics.velocity += toWorld({THRUST * dt, 0.0f});
        anim.current_state  = Game::Core::AnimationState::RUN;
        sound.current_state = Game::Core::SoundState::RUN;
    }

    if (input.isKeyHeld(Engine::Input::Key::LEFT)) {

        physics.angularVelocity -= TURN_ACCEL * dt;
    }

    if (input.isKeyHeld(Engine::Input::Key::RIGHT)) {

        physics.angularVelocity += TURN_ACCEL * dt;
    }
}


void PlayerSystem::returnFromHyperspace_(Engine::Core::Context& ctx, Engine::Core::GameObject& player, ShipComponent& ship, float dt) {

    using Engine::Core::Random;

    ship.timer -= dt;
    if (ship.timer > 0.0f) return;

    // Anywhere, even next to an asteroid: that's the gamble of hyperspace
    const glm::vec2 screen = glm::vec2(ctx.get<Engine::Graphics::RenderSystem>().getScreenSize());

    auto& transform    = player.getComponent<Engine::Core::TransformComponent>();
    transform.position = {Random::getFloat(HYPERSPACE_MARGIN, screen.x - HYPERSPACE_MARGIN), Random::getFloat(HYPERSPACE_MARGIN, screen.y - HYPERSPACE_MARGIN)};

    // ... and sometimes the ship doesn't survive the jump. It explodes where it would have appeared,
    // through the same event as an asteroid hit: the life, the bang and the respawn happen as usual.
    if (Random::getFloat(0.0f, 1.0f) < HYPERSPACE_RISK) {
        ctx.get<Engine::Events::EventBus>().publish<Events::ShipDestroyedEvent>(transform.position);
        ship.status = ShipComponent::Status::DESTROYED;  // it stays switched off
        ship.timer  = RESPAWN_DELAY;
        return;
    }

    auto& physics           = player.getComponent<Engine::Physics::PhysicsComponent>();
    physics.velocity        = {0.0f, 0.0f};
    physics.angularVelocity = 0.0f;

    player.setActive(true);
    ship.status = ShipComponent::Status::FLYING;
}


void PlayerSystem::waitForRespawn_(Engine::Core::Context& ctx, Engine::Core::GameObject& player, ShipComponent& ship, float dt) {

    if (ctx.get<Game::Core::GameSession>().lives <= 0) return;  // game over: this ship stays gone

    ship.timer -= dt;
    if (ship.timer > 0.0f) return;

    // Like the arcade: rather than making the new ship invulnerable for a while, it simply waits
    // until no asteroid is near the middle of the screen, where it appears
    const glm::vec2 center = glm::vec2(ctx.get<Engine::Graphics::RenderSystem>().getScreenSize()) / 2.0f;
    if (!isAreaClear(ctx, center, SAFE_RADIUS)) return;

    auto& transform    = player.getComponent<Engine::Core::TransformComponent>();
    transform.position = center;
    transform.rotation = SPAWN_ROTATION;

    auto& physics           = player.getComponent<Engine::Physics::PhysicsComponent>();
    physics.velocity        = {0.0f, 0.0f};
    physics.angularVelocity = 0.0f;

    player.setActive(true);
    ship.status = ShipComponent::Status::FLYING;
}
}  // namespace Game::Player
