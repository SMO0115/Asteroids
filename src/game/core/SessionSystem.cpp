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

// The arcade's points: the smaller the asteroid, the harder to hit, the more it's worth
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

    for (const auto& destroyed : event_bus.get<Events::AsteroidDestroyedEvent>()) {

        session.score += pointsFor(destroyed.size);

        // 'while', not 'if': in theory one tick could pass two thresholds
        while (session.score >= session.next_extra_life) {
            session.lives++;
            session.next_extra_life += GameSession::EXTRA_LIFE_EVERY;
            event_bus.publish<Engine::Events::PlaySoundEvent>("extraShip");
        }
    }

    // A ship hit counts once, even if it touched two asteroids in the same tick: AsteroidSystem
    // switches the ship off at the first hit, and ignores hits by objects that are already off
    for ([[maybe_unused]] const auto& hit : event_bus.get<Events::ShipDestroyedEvent>()) {
        session.lives--;
        LOG_INFO("Ship destroyed, lives left: {}", session.lives);
    }
}
}  // namespace Game::Core
