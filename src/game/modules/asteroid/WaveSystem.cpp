//
// Created by smo0115 on 10/24/26.
//

#include "WaveSystem.h"

#include <algorithm>
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "AsteroidComponent.h"
#include "engine/core/CoreModule.h"
#include "engine/events/EngineEvents.h"
#include "engine/events/EventBus.h"
#include "engine/modules/graphics/RenderSystem.h"
#include "engine/utils/Random.h"
#include "game/core/GameSession.h"
#include "game/core/GameTypes.h"
#include "game/modules/player/ShipComponent.h"

namespace Game::Asteroid {

constexpr float WAVE_DELAY      = 2.0f;    // seconds of empty sky between two waves
constexpr int   FIRST_WAVE_SIZE = 4;       // large asteroids in wave 1 ...
constexpr int   WAVE_GROWTH     = 2;       // ... 2 more in every next wave ...
constexpr int   MAX_WAVE_SIZE   = 11;      // ... but never more than 11 (the arcade's limit)
constexpr float MIN_SPEED       = 40.0f;   // px/s
constexpr float MAX_SPEED       = 100.0f;  // px/s
constexpr float SAFE_DISTANCE   = 200.0f;  // px: a new asteroid never appears this close to the ship

constexpr float BEAT_SLOWEST = 1.0f;   // seconds between beats when a wave starts ...
constexpr float BEAT_SPEEDUP = 0.02f;  // ... a little less after every beat ...
constexpr float BEAT_FASTEST = 0.25f;  // ... down to this (after ~40 beats, about 25 s)


namespace {
// Like the arcade, the heartbeat only plays while a ship is in play: it stops when the ship is
// destroyed and goes on when the next one appears
bool shipInPlay(Engine::Core::Context& ctx) {

    const Engine::Core::Pool& player = ctx.get<Engine::Core::Scene>().getPool(Game::Core::GamePools::PLAYER);
    if (player.empty()) return false;

    return player[0]->getComponent<Player::ShipComponent>().status != Player::ShipComponent::Status::DESTROYED;
}
}  // namespace


void WaveSystem::reset() {

    m_next_wave_timer = 0.0f;
    restartBeat_();
}


void WaveSystem::update(Engine::Core::Context& ctx, float dt) {

    Engine::Core::Pool& enemies = ctx.get<Engine::Core::Scene>().getPool(Game::Core::GamePools::ENEMIES);

    // An asteroid that was destroyed is already inactive, so 'active' is all we need to check
    const bool sky_is_empty = std::none_of(enemies.begin(), enemies.end(), [](const std::unique_ptr<Engine::Core::GameObject>& object) {
        return object->isActive() && object->hasComponent<AsteroidComponent>();
    });

    if (!sky_is_empty) {
        m_next_wave_timer = WAVE_DELAY;  // so the pause starts when the LAST asteroid goes
        if (shipInPlay(ctx)) beat_(ctx, dt);
        return;
    }

    m_next_wave_timer -= dt;
    if (m_next_wave_timer > 0.0f) return;

    auto&     session = ctx.get<Game::Core::GameSession>();
    const int count   = std::min(FIRST_WAVE_SIZE + WAVE_GROWTH * session.wave, MAX_WAVE_SIZE);
    session.wave++;
    LOG_INFO("Wave {}: {} asteroids", session.wave, count);

    // Keep away from the ship, wherever it is now. (No ship: keep away from the screen center.)
    glm::vec2                 avoid  = glm::vec2(ctx.get<Engine::Graphics::RenderSystem>().getScreenSize()) / 2.0f;
    const Engine::Core::Pool& player = ctx.get<Engine::Core::Scene>().getPool(Game::Core::GamePools::PLAYER);
    if (!player.empty()) avoid = player[0]->getComponent<Engine::Core::TransformComponent>().position;

    spawnAsteroids(ctx, count, avoid);
    restartBeat_();  // every wave starts slow again
}


void WaveSystem::beat_(Engine::Core::Context& ctx, float dt) {

    m_beat_timer -= dt;
    if (m_beat_timer > 0.0f) return;

    ctx.get<Engine::Events::EventBus>().publish<Engine::Events::PlaySoundEvent>(m_beat_second ? "beat2" : "beat1");
    m_beat_second = !m_beat_second;

    m_beat_interval = std::max(m_beat_interval - BEAT_SPEEDUP, BEAT_FASTEST);
    m_beat_timer += m_beat_interval;  // +=, not =: the time this tick overshot isn't lost, so the rhythm stays even
}


void WaveSystem::restartBeat_() {

    m_beat_interval = BEAT_SLOWEST;
    m_beat_timer    = BEAT_SLOWEST;
    m_beat_second   = false;
}


void WaveSystem::spawnAsteroids(Engine::Core::Context& ctx, int count, glm::vec2 avoid) {

    using Engine::Core::Random;

    const glm::vec2 screen  = glm::vec2(ctx.get<Engine::Graphics::RenderSystem>().getScreenSize());
    auto&           scripts = ctx.get<Engine::Scripting::ScriptManager>();

    for (int i = 0; i < count; ++i) {

        // A random point on a random edge. Try again if it's too close to the ship; give up after a
        // few tries (a ship flying along an edge could block it), rather than looping forever.
        glm::vec2 position;
        for (int tries = 0; tries < 10; ++tries) {
            const float     along   = Random::getFloat(0.0f, 1.0f);
            const glm::vec2 edges[] = {
                {along * screen.x, 0.0f},      // top
                {along * screen.x, screen.y},  // bottom
                {0.0f, along * screen.y},      // left
                {screen.x, along * screen.y},  // right
            };
            position = edges[static_cast<std::size_t>(Random::getInt(0, 3))];
            if (glm::distance(position, avoid) >= SAFE_DISTANCE) break;
        }

        const float     angle    = glm::radians(Random::getFloat(0.0f, 360.0f));
        const glm::vec2 velocity = glm::rotate(glm::vec2(Random::getFloat(MIN_SPEED, MAX_SPEED), 0.0f), angle);

        scripts.call("SpawnAsteroid", position.x, position.y, velocity.x, velocity.y, 3);
    }
}
}  // namespace Game::Asteroid
