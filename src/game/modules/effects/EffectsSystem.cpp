//
// Created by smo0115 on 10/24/26.
//

#include "EffectsSystem.h"

#include <glm/gtx/rotate_vector.hpp>

#include "engine/core/CoreModule.h"
#include "engine/events/EngineEvents.h"
#include "engine/events/EventBus.h"
#include "engine/utils/Random.h"
#include "game/events/GameEvents.h"

namespace Game::Effects {

// Per asteroid size (index = size): bigger rocks make a deeper bang and more debris
constexpr const char* BANG[]   = {"", "bangSmall", "bangMedium", "bangLarge"};
constexpr int         PIECES[] = {0, 5, 8, 12};


void EffectsSystem::update(Engine::Core::Context& ctx) {

    auto& event_bus = ctx.get<Engine::Events::EventBus>();

    for (const auto& destroyed : event_bus.get<Events::AsteroidDestroyedEvent>()) {

        const int size = destroyed.size >= 1 && destroyed.size <= 3 ? destroyed.size : 1;  // never index out of the arrays

        event_bus.publish<Engine::Events::PlaySoundEvent>(BANG[size]);
        burst_(ctx, destroyed.position, PIECES[size], 150.0f, 0.8f);
    }

    for (const auto& destroyed : event_bus.get<Events::SaucerDestroyedEvent>()) {

        event_bus.publish<Engine::Events::PlaySoundEvent>("bangLarge");
        burst_(ctx, destroyed.position, destroyed.small ? 10 : 16, 180.0f, 1.0f);
    }

    // The ship: more pieces that fly farther and last longer, so the loss is easy to see
    for (const auto& destroyed : event_bus.get<Events::ShipDestroyedEvent>()) {

        event_bus.publish<Engine::Events::PlaySoundEvent>("bangLarge");
        burst_(ctx, destroyed.position, 24, 220.0f, 1.6f);
    }
}


void EffectsSystem::burst_(Engine::Core::Context& ctx, glm::vec2 position, int pieces, float max_speed, float max_lifetime) {

    using Engine::Core::Random;

    auto& scripts = ctx.get<Engine::Scripting::ScriptManager>();

    for (int i = 0; i < pieces; ++i) {

        // Random direction and speed, so no two explosions look the same
        const float     angle    = glm::radians(Random::getFloat(0.0f, 360.0f));
        const glm::vec2 velocity = glm::rotate(glm::vec2(Random::getFloat(max_speed * 0.3f, max_speed), 0.0f), angle);
        const float     lifetime = Random::getFloat(max_lifetime * 0.4f, max_lifetime);

        scripts.call("SpawnDebris", position.x, position.y, velocity.x, velocity.y, lifetime);
    }
}
}  // namespace Game::Effects
