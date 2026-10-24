//
// Created by smo0115 on 10/24/26.
//

#include "SessionSystem.h"

#include "GameSession.h"
#include "engine/core/CoreModule.h"
#include "engine/core/Log.h"
#include "engine/events/EngineEvents.h"
#include "engine/events/EventBus.h"
#include "game/events/GameEvents.h"

namespace Game::Core {

// The arcade's points: the smaller the target, the harder to hit, the more it's worth
constexpr int LARGE_SAUCER_POINTS = 200;
constexpr int SMALL_SAUCER_POINTS = 1000;

constexpr int pointsFor(int asteroid_size) {
    switch (asteroid_size) {
        case 3:
            return 20;
        case 2:
            return 50;
        default:
            return 100;
    }
}


void SessionSystem::update(Engine::Core::Context& ctx) {

    auto& event_bus = ctx.get<Engine::Events::EventBus>();
    auto& session   = ctx.get<GameSession>();

    // Only what the player destroyed scores (not an asteroid that a saucer shot)
    for (const auto& destroyed : event_bus.get<Events::AsteroidDestroyedEvent>()) {
        if (destroyed.by_player) session.score += pointsFor(destroyed.size);
    }
    for (const auto& destroyed : event_bus.get<Events::SaucerDestroyedEvent>()) {
        if (destroyed.by_player) session.score += destroyed.small ? SMALL_SAUCER_POINTS : LARGE_SAUCER_POINTS;
    }

    // 'while', not 'if': in theory one tick could pass two thresholds
    while (session.score >= session.next_extra_life) {
        session.lives++;
        session.next_extra_life += GameSession::EXTRA_LIFE_EVERY;
        event_bus.publish<Engine::Events::PlaySoundEvent>("extraShip");
    }

    // A ship hit counts once, even if it touched two asteroids in the same tick: the CombatSystem
    // switches the ship off at the first hit, and ignores hits by objects that are already off
    for ([[maybe_unused]] const auto& hit : event_bus.get<Events::ShipDestroyedEvent>()) {
        session.lives--;
        LOG_INFO("Ship destroyed, lives left: {}", session.lives);
    }
}
}  // namespace Game::Core
